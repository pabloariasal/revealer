# revealer

Gathers system data and files upon file creation events.

# Build

## System Dependencies

* CMake (>=3.16)
* Boost
* Python3 (for application tests)

## Build
```
git clone https://github.com/pabloariasal/revealer.git
cd revealer
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

# Run Unit tests

```sh
ctest --test-dir build
```

# Run Application Tests

Application tests are written in python. `pipenv` is used to manage the environment.

## Install `pipenv`

```sh
pip install --user pipenv
```

## Create Virtual Environment

```sh
cd revealer
pipenv sync
```

## Run the application tests
```sh
pipenv run pytest application_tests --test-exec <path_to_revealer_exec>
```

# Usage

```sh
revealer --observe-directory <directory_to_be_observed> --output-directory <output_dir> [paths...]
```
