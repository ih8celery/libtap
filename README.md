version 1.1.0.1

#What is libtap++?

  libtap++ is a simple library providing a test harness to C++ programs of all sizes.
it conforms to the Test Anything Protocol (TAP for short) originally created for
perl.

  see the current TAP specification [here](https://testanything.org/tap-version-13-specification.html) or look below to the API section to see                                                       a description of the salient features of the implementation. 

  libtap++ was created by [Leon Timmermans](https://github.com/Leont); the code hosted here is a
maintenance version developed to address some problems in the original implementation.

#Why I forked Leont's project
  I have taken up maintenance of this library, formerly part
of [libperl++](https://github.com/Leont/libperl--), from the original author, Leon Timmermans, who appears to have
abandoned this project in late 2011. I have no association with Mr. Timmermans,
and I did not seek his approval or advice before forking it. coming from Perl 5 to 
C++, I discovered his work through the official TAP website. in the state he left it, 
I believed the project was incomplete. specifically, I was disappointed to find
that the codebase was out-of-date, tied to two unrelated projects, and
directed more to perl users than to C++ programmers. after considering my schedule,
I decided to maintain libtap++ for myself.

  Since then my goals in maintaining it have matured into the following:
1. to keep the code up to date with the C++11 standard
2. to keep the project conformant with the latest TAP specification (TAP 13)
3. ~~to move the build process from a custom perl build to more "native" C++ build tools~~
4. ~~to remove non-standard libraries (read: Boost) from the dependencies~~
5. to develop and test libtap++ for multiple platforms
6. to improve the documentation
7. to take requests for non-TAP extensions that advantageously employ C++ features.

  I believe TAP is an important innovation in testing that C++ programmers should
be able to enjoy--on their own terms.

#The Details

  note that some details and documentation as referenced here are incomplete or
unavailable. this is a work in progress.

  as development progresses, [userman](./doc/userman.md) will be updated with complete 
information about all parts of the project.

##API
```c++
plan(unsigned)
plan(details::skill_all_type, std::string& = "")
plan(details::no_plan_type)

  declare that tests will be run

unsigned planned()
unsigned encountered()

ok(bool condition, std::string& = "")

  pass test if condition is true

not_ok(bool, std::string& = "")

  pass test if condition is false

template <typename T, typename U> bool is(T&, U&, std::string&)
      
  pass test if first two arguments have equal values same type

template <typename T, typename U> bool isnt(T&, U&, std::string&)
      
   pass test if first two arguments differ in type and/or value

pass(std::string& = "")

  automatically pass test

fail(std::string& = "")

  automatically fail test

skip(unsigned, std::string& = "")

  declare that n tests will be skipped

note(std::string)

  append useful information to the output stream

diag(std::string)

  append a message to the output of current test without disrupting
its basic output

done_testing()
done_testing(unsigned)
bail_out(std::string& = "")

  announces to the harness that testing is complete

exit_status()

  returns the exit status, which is the number of tests failed in the
current plan or 255, whichever is the lesser

set_output(std::ostream)

  sets the stream used to report test results

set_error(std::ostream)

  sets the stream used to report errors
```

  Extras:
```c++
TEST_START(int n)

  macro similar to plan

TEST_END()

  macro similar to done_testing

TRY(statement, std::string description)

  macro that catches exceptions from executing a statement and
prints test description

TRY_DECL(declaration, std::string, message)

  macro that catches exceptions from executing an assignment
and prints test description

FAIL(action, std::string)

```
##Usage Notes

  TAP is delivered to you in the TAP namespace, which is generally
safe to `use`.

  The test extras are accessible by `#define`ing WANT\_TEST\_EXTRAS
before including the header. Use the extras when you want to catch
exceptions within your tests.

// TODO when to choose one function over another
// TODO links to sites explaining test-driven development

##Dependencies

libtap++ will have no non-standard dependencies.

#Installation

##Ubuntu
  I recommend you do an out-source cmake build, but it is not necessary.
To do this, start with the following in your shell from the project:
  ```shell
    mkdir build && cd build
    cmake ..
  ```

After these steps, you can use make:
  ```shell
    make
    make run_tests
    sudo make install
  ```

##Mac OS X
  Mac is not yet supported

##Windows
  Windows is not yet supported.

  for advanced installation information, see [install](./doc/install.md).

#Issues
  please contact me about building and testing for your system if it
is not currently supported.

  if you have an issue installing or running this software, please
read the information in [creating-issues](./doc/creating-issues.md) and [FAQs](./doc/FAQs.md) before
creating a Github issue, and be sure you have read the docs relevant
to what you are doing (see above).

#COPYRIGHT AND LICENCE

(how can I modify this license)
(remove references to perl)

Copyright (C) 2007, 2008, 2009, 2010 Leon Timmermans
This program is free software; you can redistribute it and/or modify it
under the same terms as Perl itself.
