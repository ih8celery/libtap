# TAP's Core
at a minimum, libtap++ will do everything that the most recent TAP
specification prescribes. if I ever decide to diverge from TAP, I
will rename the project to something more appropriate. beyond the
protocol, though, libtap++ should provide some capabilities that 
only make sense to a C++ programmer. to detail how I wish libtap++
to evolve in this direction, I must first explain my view of what
is central to TAP itself.

TAP is a simple -- one might fairly term it 'minimal'-- and flexible 
protocol that produces readable output. all of TAP is built atop 
the ok/not\_ok pair of testing functions, which is reflected in
TAP's output. in a sense, tests are 'autovivified' as soon as a test
function is called, so the final argument to test functions is less
a comment about and more an identifier for the test. TAP has but
a few testing functions which can be categorized into two families
of tests:
* tests of conditions (represented by ok and not\_ok)
* tests of comparisons (represented by is and isnt; in perl, this
  family may also include a like function that uses regular expressions)

obviously, the second category of tests can be implemented entirely with
ok and not\_ok, but the comparison tests provide a convenient means to
perform a common type of testing, often reducing the burden on the
programmer. the benefit of a separate category therefore outweighs the
need for simplicity.

TAP associates a "state" with every test which influences the output
produced for it: a normal test is run and prints "ok" or "not ok" at
the start of its output; a skipped test replaces that text with "skip";
and a todo test uses "TODO". these alternative states are readily
integrated into test-driven development, making TAP a suitable tool
for feature planning.

# Extension Requirements
the following two sections detail the basic requirements for the possible
extensions to libtap++. I won't consider changing anything else except
to fix a bug.

## Extending the Tests
every addition to the set of test functions should fit into one of the
aforementioned categories both semantically and "morphologically".
this means that a function will test only strictly within its category
and will contain the category as part of its name. furthermore, this
means that new test functions should occur in pairs. these rules
provide the benefit that the behavior and arguments to a function are
predictable at a glance. `is_convertible`, for instance, is a template
function that compares two types using `std::is_convertible`. `TRY_OK`
and `TRY_NOT_OK` are macros which test a special kind of condition --
whether the first argument throws an exception. `TRY_OK` fails if
an exception is thrown, and `TRY_NOT_OK` does the opposite.

libtap++'s test functions are easily distinguished from other types
of functions used by the library since they are the only functions
allowed to contain "is" or "ok" in their names. capitalization is
required for macros. words should be delimited by a hyphen.

## Extending the Printer
TAP emits output of four kinds: 
1. test lines
2. plans
3. notes
4. error messages

// TODO finish explaining how each output may be customized

# Additional Information
please read [patches](./creating-patches.md) before attempting to submit your implementation
of an extension to TAP.
