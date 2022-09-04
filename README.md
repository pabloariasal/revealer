# revealer - Data collector

Gathers system data upon file creation events.

# Build

## System Dependencies

* CMake
* Boost
* python3 (for application tests)

## Build
```
git clone https://github.com/pabloariasal/revealer.git
cd revealer
cmake -Bbuild
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
pipenv run pytest application_tests --test-exec <revealer_executable_to_test>
```
e.g.

```sh
pipenv run pytest application_tests --test-exec build/revealer
```
