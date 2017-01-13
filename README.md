[![Build Status](https://travis-ci.org/unittest-cpp/unittest-cpp.svg?branch=master)](https://travis-ci.org/unittest-cpp/unittest-cpp)
[![Build status](https://ci.appveyor.com/api/projects/status/ffs2k8dddts5cyok/branch/master?svg=true)](https://ci.appveyor.com/project/pjohnmeyer/unittest-cpp/branch/master)

UnitTest++
===========

UnitTest++ is a lightweight unit testing framework for C++. It was designed to do test-driven development on a wide variety of platforms. Simplicity, portability, speed, and small footprint are all very important aspects of UnitTest++. UnitTest++ is mostly standard C++ and makes minimal use of advanced library and language features, which means it should be easily portable to just about any platform. Out of the box, the following platforms are supported:

* Windows
* Linux
* Mac OS X

Documentation
--------------
The full documentation for building and using UnitTest++ can be found on the [GitHub wiki page](https://github.com/unittest-cpp/unittest-cpp/wiki). The contents of this wiki are also included as a git submodule under the `docs` folder, so version-specific Markdown documentation is always available along with the download.

Pre-requisites
---------------
While there are currently some bundled automake files, UnitTest++ is primarily built and supported using [CMake](http://cmake.org).

Downloading
------------
### Latest (HEAD) ###

Via git:

    git clone https://github.com/unittest-cpp/unittest-cpp

Via svn:

    svn checkout https://github.com/unittest-cpp/unittest-cpp/trunk unittest-cpp

### Latest release (v2.0.0) ###

Via git:

    git clone https://github.com/unittest-cpp/unittest-cpp
    cd unittest-cpp
    git checkout v2.0.0

Via svn:

    svn checkout https://github.com/unittest-cpp/unittest-cpp/tags/v2.0.0 unittest-cpp

License
---------
*UnitTest++ is free software. You may copy, distribute, and modify it under
the terms of the License contained in the file LICENSE distributed
with this package. This license is the same as the MIT/X Consortium
license.*

Contributors
--------------
### [GitHub Contributor Graph](https://github.com/unittest-cpp/unittest-cpp/contributors) ###

### Current Maintainers: ###
* Patrick Johnmeyer (pjohnmeyer@gmail.com) @pjohnmeyer
* Charles Nicholson (charles.nicholson@gmail.com) @charlesnicholson

### Original Authors: ###
* Noel Llopis (llopis@convexhull.com)
* Charles Nicholson (charles.nicholson@gmail.com)

### Contributors not included in github history ###
* Jim Tilander
* Kim Grasman
* Jonathan Jansson
* Dirck Blaskey
* Rory Driscoll
* Dan Lind
* Matt Kimmel -- Submitted with permission from Blue Fang Games
* Anthony Moralez
* Jeff Dixon
* Randy Coulman
* Lieven van der Heide

Historic release notes
----------------------

### Version 2.0.0 (2017-01-13) ###
- Change Check method supporting CHECK macro to accept argument by reference
- Introduce long macro forms (e.g. UNITTEST_CHECK); make short forms optional
- Improved Visual Studio 2015 support
- [Full List](https://github.com/unittest-cpp/unittest-cpp/issues?q=milestone%3A2.0.0+)

### Version 1.6.0 (2016-02-29) ###
- Add REQUIRE macro to end tests early when selected checks fail
- [Full List](https://github.com/unittest-cpp/unittest-cpp/issues?q=milestone%3A1.6.0+)

### Version 1.5.1 (2016-01-30) ###
- pkg-config support
- Fix for Visual Studio 2010 compilation issue in 1.5.0
- [Full List](https://github.com/unittest-cpp/unittest-cpp/issues?q=milestone%3A1.5.1+)

### Version 1.5 (2015-11-04) ###
- Visual Studio 2015 support
- CMake-based build management
- Integration of SourceForge and Google Code versions of the project
- [Full List](https://github.com/unittest-cpp/unittest-cpp/issues?q=is%3Aissue+is%3Aclosed+milestone%3A1.5.0)

### Version 1.4 (2008-10-30) ###
- CHECK macros work at arbitrary stack depth from inside TESTs.
- Remove obsolete TEST_UTILITY macros
- Predicated test execution (via TestRunner::RunTestsIf)
- Better exception handling for fixture ctors/dtors.
- VC6/7/8/9 support

### Version 1.3 (2007-4-22) ###
- Removed dynamic memory allocations (other than streams)
- MinGW support
- Consistent (native) line endings
- Minor bug fixing

### Version 1.2 (2006-10-29) ###
- First pass at documentation.
- More detailed error crash catching in fixtures.
- Standard streams used for printing objects under check. This should allow the
  use of standard class types such as std::string or other custom classes with
  stream operators to ostream.
- Standard streams can be optionally compiled off by defining UNITTEST_USE_CUSTOM_STREAMS
  in Config.h
- Added named test suites
- Added CHECK_ARRAY2D_CLOSE
- Posix library name is libUnitTest++.a now
- Floating point numbers are postfixed with 'f' in the failure reports

### Version 1.1 (2006-04-18) ###
- CHECK macros do not have side effects even if one of the parameters changes state
- Removed CHECK_ARRAY_EQUAL (too similar to CHECK_ARRAY_CLOSE)
- Added local and global time constraints
- Removed dependencies on strstream
- Improved Posix signal to exception translator
- Failing tests are added to Visual Studio's error list
- Fixed Visual Studio projects to work with spaces in directories

### Version 1.0 (2006-03-15) ###
- Initial release

