# C++ Project with CI/CD testing pipeline

This repository contains a C++ project with multiplatform CI/CD pipeline that builds the project using CMake and runs unit tests via GoogleTest on every push or pull request to `master`.

It also contains a `docker-compose.yml`  file that runs a Bash script to build the program, execute the tests, and redirect the output to a mounted volume.
