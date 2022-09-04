import pytest
import subprocess
import tarfile
import time
from pathlib import Path

FILES_TO_INCLUDE_IN_TARBALL = [('foo/hello.txt', 'Hello MBTI :)'),
                               ('bar.md', 'bar!\n'),
                               ('foo/bar/baz.txt', 'baz...')]

# None contents means that we don't care about the contents of the file,
# only that the final tarball contains a  (non-empty) file exists at the specified path
DISK_USAGE_FILE = (Path('disk_usage.txt'), None)

# time to wait after a file has been created and the service creates a tarball
WAIT_TIME_SECONDS = 0.5


def create_files(files):
    for path, contents in files:
        path.parent.mkdir(parents=True, exist_ok=True)
        with open(path, 'w') as f:
            f.write(contents)


@pytest.fixture(scope="session")
def revealer_exec(pytestconfig):
    return pytestconfig.getoption("--test-exec")


@pytest.fixture
def revealer_service(revealer_exec, tmp_path_factory):
    '''
    starts the revealer service
    returns the observed_dir, output_dir, and files to be included in the tarball
    a file is a tuple of (file_path: Path, file_contents: str)
    '''
    dummy_fs_root = tmp_path_factory.mktemp('fs_root')
    # make paths absolute
    files = [(dummy_fs_root / path, contents)
             for path, contents in FILES_TO_INCLUDE_IN_TARBALL]
    create_files(files)

    output_dir = tmp_path_factory.mktemp('output') / 'tarballs'
    observed_dir = tmp_path_factory.mktemp('observed')
    paths_to_include = [file for file, _ in files]
    popen = subprocess.Popen([revealer_exec,
                              "--observe-directory", observed_dir,
                              "--output-directory", output_dir,
                              *paths_to_include],
                             stdout=subprocess.PIPE,
                             encoding='utf-8')
    yield observed_dir, output_dir, files
    try:
        popen.communicate(timeout=0.1)
    except subprocess.TimeoutExpired:
        popen.kill()
        stdout, _ = popen.communicate()
        print()
        print("### output ###")
        print(stdout)


def check_tarball_valid(tarball_path, expected_files):
    '''
    Checks that the tarball in `tarball_path` contains only the files listed in `expected_files`
    a file is repreented as a tuple (file: Path, contents: str)
    '''
    assert tarfile.is_tarfile(tarball_path)
    with tarfile.open(tarball_path, 'r') as t:
        files_in_tarball = t.getmembers()
        assert len(files_in_tarball) == len(expected_files)
        for (expected_name, expected_content) in expected_files:
            # get the matching member in the tarball, quadratic time but irrelevant in this case
            matching = next(
                (m for m in files_in_tarball if m.name == expected_name.as_posix()), None)
            assert matching, f"the tarball doesn not contain expected file '{expected_name}'"
            with t.extractfile(matching) as m:
                actual_content = m.read()
                assert len(actual_content) > 0, f"the file '{matching.name}' is empty"
                if expected_content:
                    assert actual_content.decode('UTF-8') == expected_content


def tarballs_in_dir(dir):
    return list(dir.glob('*.tar'))


def test_revealer(revealer_service):
    observed_dir, output_dir, files_to_include = revealer_service
    assert len(tarballs_in_dir(output_dir)) == 0

    # Create a file that doesn't match the pattern, nothing should happen
    (observed_dir / 'not_matching_file.txt').touch()
    time.sleep(WAIT_TIME_SECONDS)
    assert len(tarballs_in_dir(output_dir)) == 0

    # Create a file that matches the pattern
    # a tarball with the expected files should be saved under output_dir
    (observed_dir / 'core.lz4').touch()
    time.sleep(WAIT_TIME_SECONDS)
    tarballs = tarballs_in_dir(output_dir)
    assert len(tarballs) == 1
    first_tarball = tarballs[0]
    check_tarball_valid(first_tarball, files_to_include + [DISK_USAGE_FILE])

    # Create another file that matches the pattern
    # a new tarball should be saved under output_dir
    (observed_dir / 'core.ffff.lz4').touch()
    time.sleep(WAIT_TIME_SECONDS)
    tarballs = tarballs_in_dir(output_dir)
    assert len(tarballs) == 2
    second_tarball = next(t for t in tarballs if t != first_tarball)
    check_tarball_valid(second_tarball, files_to_include + [DISK_USAGE_FILE])
