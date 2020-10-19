# KSeExpr - An embeddable expression evaluation engine

This repository hosts the fork of [Disney Animation's SeExpr expression 
library](https://wdas.github.io/SeExpr), that is used in [Krita](https://invent.kde.org/graphics/krita).

This fork was created as part of the GSoC 2020 project, [Dynamic Fill 
Layers in Krita using SeExpr](https://summerofcode.withgoogle.com/archive/2020/projects/6233717216903168/),
to enable the provision of fixes and translations needed to embed 
SeExpr into the Krita painting suite.

**WARNING**: This version is not ABI-compatible with projects using upstream SeExpr. Use it at your own discretion.

## Super impatient CMake building and installing guide

Prerequisites:

* A C++ compiler supporting **at least** the C++14 standard
* CMake >= 3.8.0
* Qt >= 5.9

```bash
$ mkdir kseexpr
$ cd kseexpr
$ git clone https://invent.kde.org/graphics/seexpr.git src
$ mkdir build
$ cd build
$ cmake ../src
$ cmake --build . --target install
```

## The full package

If you want optional stuff (not used in Krita):
* Ninja
* For building the parser, otherwise specify `-DUSE_PREGENERATED_FILES=OFF`:
  * bison
  * flex
* For LLVM support, otherwise specify `-DENABLE_LLVM_BACKEND=OFF`:
  * LLVM >= 3.8.0
* For building the documentation, otherwise specify `-DBUILD_DOC=OFF`:
  * doxygen
* For building the tests, otherwise specify `-DBUILD_TESTS=OFF`:
  * gtest
* For building the translations (if you have a release tarball, you can disable it with `-DBUILD_TRANSLATIONS=OFF`):
  * ECM >= 5.64

Then, on your shell, issue the following commands:

```bash
$ mkdir kseexpr
$ cd kseexpr
$ git clone https://invent.kde.org/graphics/seexpr.git src
$ mkdir build
$ cd build
$ cmake ../src
$ cmake --build . --target install
```

You can try the test application asciiGraph which is a simple
function grapher... e.g.
```bash
./asciiGraph "x^3-8*x"
```

## Getting started

Examining the demo applications in `src/demos` is a great way to see
some usage examples of the library. The basic strategy is to subclass
the `Expression` class and implement the methods that describe what
extra functions and variables your expression evaluation will need.

## Documentation

We (the Krita team) provided a set of tutorials, an adapted Quick Reference, and
other docs at [the Krita manual](https://docs.krita.org/en/tutorials/seexpr.html).

The original Doxygen-based documentation provided by Disney is at:
- <http://wdas.github.io/SeExpr/doxygen/>
- <http://wdas.github.io/SeExpr/doxygen/v1> (for older documentation)

## Source code overview

```
cmake/       CMake boilerplate and macros
generated/   Pregenerated parser files
LICENSES/    SPDX licenses
packaging/   Krita packaging scripts
po/          (only on release tarballs) Translations supplied by the KDE i18n team
src/
  build/     CMake helper macros
  demos/     Demo Applications
  doc/       Doxygen generation
  KSeExpr/   Library code
  KSeExprUI/ User Interface components for editing
  tests/     Regression Tests
  utils/     Various example tools
```

## License

KSeExpr is made available subject to the terms of the [GNU General Public License v3.0 or later](LICENSES/GPL-3.0-or-later.txt).

It is based on code from SeExpr, which is (C) Disney Enterprises, Inc. and made available under the Apache 2.0 License with section 6 rewrite. Please see the [LICENSE](LICENSES/LicenseRef-Apache-2.0.txt) for more details.

The translation loader is a modified version of the one provided by k18n under the [BSD 3-Clause "New" or "Revised" License](LICENSES/BSD-3-Clause.txt).

The `crack_atof` function by Tian Bo (in Utils.cpp) is licensed under the [MIT](LICENSES/MIT.txt).

The packaging scripts come from Krita and are licensed under the [GNU General Public License v2.0](LICENSES/GPL-2.0.txt).


## Original SeExpr Developers

 * David Aguilar
 * Janet Berlin
 * Brent Burley
 * Lawrence Chai
 * Noah Kagan
 * Jared Reisweber
 * Andrew Selle
 * Dan Teece
 * Tom Thompson

Walt Disney Animation Studios
