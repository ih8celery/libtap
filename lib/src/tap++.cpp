#define WANT_TEST_EXTRAS
#include "tap++.h"
#include <stack>
#include <cstdlib>

namespace TAP {
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

  namespace {
    unsigned expected = 0; // planned number of tests as initialized by plan()
    unsigned counter = 0; // count of all tests run so far
    unsigned not_oks = 0; // count of tests which fail
    
    // TODO rename to todo_description()
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

    bool is_planned = false;
    bool no_planned = false;
    bool has_output_plan = false;

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
        fail("done_testing() was already called"); // TODO replace call to fail with non-test fn
        return;
      }
      is_done = true;

      if (expected && tests != expected) {
        // TODO unmix test output from error?
        fail(std::string("planned to run ") + std::to_string(expected) + " tests but done_testing() expects " + std::to_string(tests));
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

  void done_testing() {
    _done_testing(encountered());
  }

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
    *details::output << "Bail out!  " << reason << std::endl;

    std::exit(255); // does not unwind stack!
  }

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

  void skip(unsigned num, const std::string& reason) {
    for(unsigned i = 0; i < num; ++i) {
      pass(" # skip " + reason);
    }
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

  todo_guard::todo_guard() noexcept : value(TODO) {}
  todo_guard::~todo_guard() noexcept { TODO = value; }
  
  namespace details {
    std::ostream* output = &std::cout;
    std::ostream* error = &std::cout;

    static std::stack<unsigned> block_expected;

    void start_block(unsigned expected) noexcept {
      block_expected.push(encountered() + expected);
    }
    unsigned stop_block() {
      unsigned ret = block_expected.top();
      block_expected.pop();

      return ret;
    }

    char const * failed_test_msg() noexcept {
      return is_todo_test() ? "Failed (TODO) test" : "Failed test";
    }

  }
  
  void skip(const std::string& reason) {
    throw details::Skip_exception(reason);
  }
  void skip_todo(const std::string& reason) {
    throw details::Todo_exception(reason);
  }
}
