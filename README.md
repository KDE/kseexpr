# SeExpr - An embeddable expression evaluation engine

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
$ mkdir seexpr
$ cd seexpr
$ git clone https://invent.kde.org/graphics/seexpr.git src
$ mkdir build
$ cd build
$ cmake ../src -DENABLE_LLVM_BACKEND=OFF -DENABLE_SSE4=OFF -DBUILD_DOC=OFF -DBUILD_TESTS=OFF -DBUILD_UTILS=OFF -DUSE_PYTHON=OFF
$ cmake --build . --target install
```

## The full package

If you want optional stuff (not used in Krita):
* Ninja
* For building the parser, otherwise the pregenerated files will be used:
  * bison
  * flex
* For LLVM support, otherwise specify `-DENABLE_LLVM_BACKEND=OFF`:
  * LLVM >= 3.8.0
* For the Python bindings, otherwise specify `-DUSE_PYTHON=OFF`:
  * boost::python
  * python2.7 or python3
  * PyQt5
  * SIP
* For building the documentation, otherwise specify `-DBUILD_DOC=OFF`:
  * doxygen
* For building the tests, otherwise specify `-DBUILD_TESTS=OFF`:
  * gtest
* For building the translations (if you have a release tarball, you can disable it with `-DBUILD_TRANSLATIONS=OFF`):
  * ECM >= 5.64

Then, on your shell, issue the following commands:

```bash
$ mkdir seexpr
$ cd seexpr
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
docker/     Original Disney Dockerfiles for CI
packaging/  Krita packaging scripts
po/         (only on release tarballs) Translations supplied by the KDE i18n team
src/
  build/    CMake helper macros
  demos/    Demo Applications
  doc/      Doxygen generation
  py/       Python library bindings
  SeExpr2/  Library code
    UI/       User Interface components for editing
  tests/    Regression Tests
  utils/    Various example tools
```

## License

Apache-2.0 with section 6 rewrite. Please see the [LICENSE](LICENSE) for more details.

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
