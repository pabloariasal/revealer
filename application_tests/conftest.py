import pytest


def pytest_addoption(parser):
    parser.addoption(
        '--test-exec',
        help='path to the revealer executable to be tested.',
        action='store',
        required=True)
