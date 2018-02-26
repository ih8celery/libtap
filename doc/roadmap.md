# Where is libtap++ now?
for the remainder of version 1.\*, I am focused on fully documenting
the code, adding support for Windows, completing the API, and debugging
bad behaviors. for common use cases, libtap++ works as intended, and
neither the API nor the implementation needed to change drastically.
as part of updating the code to use C++11, however, several templates
and functions were partially rewritten or deprecated. for separate
reasons, the macros `TRY` and `FAIL` have been deprecated in favor of
the similar `TRY_OK` and `TRY_NOT_OK`. the motivation for this and later-
to-come alterations and additions to libtap++ are explained [here](./extending-tap.md).

# Plans for 2.0
so what is next for libtap++?
in 2.\*, libtap++ will have a more consistent API based on the "is"
and "ok" families of test functions. it will support more variants of
these functions, including tests that compare containers and consider
specific container properties (such as size and capacity). there
should also be a complete implementation of todo tests and skip tests.
furthermore, additional controls over the tap printer are planned so
that the user may emit custom output with varying levels of verbosity
