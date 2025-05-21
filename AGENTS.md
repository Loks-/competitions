# Repo Guidelines for Codex Agents

## Overview
This repository is a header-only C++20 library of common algorithms used in
coding competitions (e.g. LeetCode or HackerRank).  All primary algorithm
implementations live under the `common/` directory.  Unit tests and benchmarking
utilities reside in `tester/`.  The project is built with CMake and provides a
set of tests that should pass before commits are finalized.

## Building and Testing
Checking if the project builds is quicker in Debug mode. Build both Debug and
Release configurations before running the tests. Only after both builds succeed
should the test suite be executed in Release mode.

```bash
# Debug build (faster for verifying compilation)
mkdir -p debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
cd ..

# Release build and tests
mkdir -p release
cd release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
ctest
```

Run these commands from the repository root before submitting a pull request.

## Coding Style
- Use modern C++20 features.  The library is intended to be header-only; new
  code should be placed in headers with `#pragma once`.
- Format source files with `clang-format` using the configuration provided in
  `.clang-format`.
- Public classes and functions should include Doxygen comments.  Files in
  `common/binary_search_tree/` (e.g. `treap.h`) demonstrate the preferred style
  for documentation, naming, and code layout.

## Directory Layout
- `common/` – algorithm implementations and utilities.
- `tester/`  – test programs executed via CTest.
- `utils/`   – helper programs.

Avoid committing build artifacts (`release/`, `debug/`, etc.) or files generated
by tests.  Follow the repository structure and style when adding new features or
updating existing ones.
