#define WANT_TEST_EXTRAS
#include "tap++.h"
#include <stack>
#include <cstdlib>

namespace TAP {
  const std::string _tap_version_info = "TAP version 13";
  /* 
   * initialized with the description of the current unimplemented test,
   * i.e. the todo test. empty string signals that test is normal 
   */
  std::string TODO = "";

  /*
   * constants used in invoking special behavior from plan function
   */
  const details::skip_all_type skip_all = details::skip_all_type();
  const details::no_plan_type no_plan = details::no_plan_type();
  
  /* private, unexported data belonging to TAP namespace */
  namespace {
    using namespace details;

    unsigned expected = 0; // planned number of tests as initialized by plan()
    unsigned counter = 0; // count of all tests run so far
    unsigned not_oks = 0; // count of tests which have failed
    Directive directive = Directive::NONE; // record current active; only one may be active at a time

    // contains the scope and reason for current "special" directive,
    // e.g. "SKIP" or "TODO"
    std::pair<unsigned, std::string> directive_info;

    // TODO deprecate 
    // and simply return TODO. initialize said variable with " # TODO " prepended
    std::string todo_test() noexcept {
      if (TODO.empty()) {
        return TODO;
      }
      else {
        return " # TODO " + TODO;
      }
    }

    /* are we currently in a test marked todo? */
    bool is_todo_test() noexcept { return !TODO.empty(); }

    bool is_planned = false; // TODO rename, means plan has been set
    bool no_planned = false; // TODO rename
    bool has_output_plan = false;

    /* TODO rename to output_plan_info */
    void output_plan(unsigned tests, const std::string& extra = "") {
      if (has_output_plan) {
        throw fatal_exception("Can't print plan twice");
      }

      *details::output << "1.." << tests << extra << std::endl;
      has_output_plan = true;
    }

    inline void _done_testing(unsigned tests) {
      static bool is_done = false;
      
      if (is_done) {
        throw fatal_exception("done_testing() was already called");
      }
      else {
        is_done = true;
      }

      if (expected != 0 && tests != expected) {
        throw fatal_exception(std::string("planned to run ")
                + std::to_string(expected)
                + " tests but done_testing() expects " 
                + std::to_string(tests));
      }
      else {
        expected = tests;
      }
      is_planned = true;
      if (!has_output_plan) {
        output_plan(tests);
      }
    }
  }

  todo_guard::todo_guard() noexcept : value(TODO) {}
  todo_guard::~todo_guard() noexcept { TODO = value; }
  
  // TODO delete this namespace
  namespace details {
    // TODO move
    std::ostream* output = &std::cout;
    std::ostream* error = &std::cout;

    // TODO move
    static std::stack<unsigned> block_expected;

    // TODO move
    void start_block(unsigned expected) noexcept {
      block_expected.push(encountered() + expected);
    }
    // TODO move
    unsigned stop_block() {
      unsigned ret = block_expected.top();
      block_expected.pop();

      return ret;
    }

    // TODO delete
    char const * failed_test_msg() noexcept {
      return is_todo_test() ? "Failed (TODO) test" : "Failed test";
    }
  }

  void plan(unsigned tests) {
    if (is_planned) {
      bail_out("Can't plan again!");
    }
    is_planned = true;

    output_plan(tests);
    expected = tests;
  }
  void plan(const details::skip_all_type&, const std::string& reason) {
    output_plan(0, " #skip " + reason);

    std::exit(0);
  }
  void plan(const details::no_plan_type&) noexcept {
    is_planned = true;
    no_planned = true;
  }

  /* TODO replace inline fn */
  void done_testing() {
    _done_testing(encountered());
  }

  /* TODO replace inline fn */
  void done_testing(unsigned tests) {
    no_planned = false;
    _done_testing(tests);
  }

  unsigned planned() noexcept {
    return expected;
  }
  unsigned encountered() noexcept {
    return counter;
  }

  int exit_status() {
    if (!is_planned && encountered()) {
      diag("Tests were run but no plan was declared and done_testing() was not seen.");
    }
    if (no_planned) {
      output_plan(encountered());
      return std::min(254u, not_oks);
    }
    else if (expected == counter) {
      return std::min(254u, not_oks);
    }
    else {
      return 255;
    }
  }
  bool summary() noexcept {
    return not_oks;
  }

  void bail_out(const std::string& reason) {
    *details::output << "Bail out! " << reason << std::endl;

    std::exit(255); // does not unwind stack!
  }

  // TODO reimplement to use separator var, description <- message,
  // test for current directive
  bool ok(bool is_ok, const std::string& message) {
    *details::output << (is_ok ? "ok " : "not ok ") << ++counter; 
    *details::output << " - " << message << todo_test()  << std::endl;

    if (!is_ok && !is_todo_test()) {
      ++not_oks;
    }

    return is_ok;
  }
  bool not_ok(bool is_not_ok, const std::string& message) {
    return !ok(!is_not_ok, message);
  }

  bool pass(const std::string& message) {
    return ok(true, message);
  }
  bool fail(const std::string& message) {
    return ok(false, message);
  }

  // mark the next num tests with skip directive
  // TODO implement correct skip
  void skip(unsigned num, const std::string& reason) {
    for(unsigned i = 0; i < num; ++i) {
      pass(" # skip " + reason);
    }
  }
  
  // mark the next test with skip directive
  // TODO implement correct skip
  void skip(const std::string& reason) {
    throw details::Skip_exception(reason);
  }

  void set_output(std::ostream& new_output) {
    if (is_planned) {
      throw fatal_exception("Can't set output after plan()");
    }
    details::output = &new_output;
  }
  void set_error(std::ostream& new_error) {
    if (is_planned) {
      throw fatal_exception("Can't set error after plan()");
    }
    details::error = &new_error;
  }
  
  /* deprecated. a test may not have more than one directive */
  void skip_todo(const std::string& reason) {
    throw details::Todo_exception(reason);
  }
}
