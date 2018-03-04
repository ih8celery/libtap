version 0.2.0

# What is libtap++?
  libtap++ is a simple library providing a test protocol (not a harness)
to C++ programs of all sizes. it conforms to the Test Anything Protocol
(TAP for short) originally created for perl. see the current TAP 
specification [here](https://testanything.org/tap-version-13-specification.html) to find out 
what TAP is all about. 

  libtap++ was created by [Leon Timmermans](https://github.com/Leont); the code hosted here is a
maintenance version developed to address some problems in the original implementation.

# Why I forked Leont's project
  I have taken up maintenance of this library, formerly part
of [libperl++](https://github.com/Leont/libperl--), from the original author, Leon Timmermans, who appears to have
abandoned this project in late 2011. I have no association with Mr. Timmermans,
and I did not seek his approval or advice before forking it. coming from Perl 5 to 
C++, I discovered his work through the official TAP website. in the state he left it, 
I believed the project was incomplete. specifically, I was disappointed to find
that the codebase was out-of-date, tied to two unrelated projects, and
directed more to perl users than to C++ programmers. since I was looking
for C++ projects, I decided to maintain libtap++ for myself.

  Since then my goals in maintaining it have matured into the following:
1. ~~to keep the code up to date with the C++11 standard~~
2. to keep the project conformant with the latest TAP specification (TAP 13)
3. ~~to move the build process from a custom perl build to more "native" C++ build tools~~
4. ~~to remove non-standard libraries (read: Boost) from the dependencies~~
5. to develop and test libtap++ for multiple platforms
6. to improve the documentation
7. to develop non-TAP extensions that advantageously employ C++ features.

  I believe TAP is an important innovation in testing that C++ programmers should
be able to enjoy--on their own terms.

# Where to Read More
  as development continues, [userman](./doc/userman.md) will be updated with complete 
information about all parts of the project. this document was originally
written by Leon Timmermans in POD format.

## Usage Notes
  TAP is delivered to you in the TAP namespace, which is generally
safe with `using` declarations.

  the test extras are accessible by `#define`ing WANT\_TEST\_EXTRAS
before including the header.

## Dependencies
libtap++ depends only on the standard libraries.

building libtap++ requires a C++ compiler with support for C++11, and 
CMake >= 3.0

# Installation

## Ubuntu
  I recommend you do an out-source cmake build. to do this, start with
the following in your shell from the project directory:
  ```shell
    mkdir build && cd build
    cmake ..
  ```
otherwise, run `cmake .` in the project directory to get started

after these steps, you can use make:
  ```shell
    make
    make test
    sudo make install
  ```

you can run more extensive tests by using `make debug` rather than `make test`.
the output of the debug tests is also neater and more verbose. running these
tests uses a shell script as a substitute for CTest

## Mac OS X
  Mac is not yet supported, as I do not have access to one for testing.
the build process will probably be similar to that used by Ubuntu.

## Windows
  Windows is being tested. instructions will be added soon (2-25-2018).

# Issues
  please contact me about building and testing for your system if it
is not currently supported.

  if you have an issue installing or running this software, please
read the information in [creating-issues](./doc/creating-issues.md) and [FAQs](./doc/FAQs.md) before
creating a Github issue, and be sure you have read the docs relevant
to what you are doing (see above).

# COPYRIGHT AND LICENCE
Copyright (C) 2007, 2008, 2009, 2010 Leon Timmermans

Copyright (C) 2018 Adam Marshall

This program is free software; you can redistribute it and/or modify it
under the same terms as Perl itself.
