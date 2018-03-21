#define WANT_TEST_EXTRAS
#include "tap++.h"
#include <stack>
#include <cstdlib>
#include <utility>

namespace TAP {
  const std::string _tap_version_info = "TAP version 13";

  namespace details {
    bool test_status = true;

    const char * separator = " - ";

    struct Test_State {
      Test_State() : expected(0), found(0), 
                     failed(0), has_plan(false),
                     directive(Directive::NONE), directive_extent(-1),
                     directive_reason() {}
      // initialize test with non-default number of test functions
      Test_State(unsigned n, bool t) : expected(n), found(0),
                                       failed(0), has_plan(t),
                                       directive(Directive::NONE),
                                       directive_extent(-1),
                                       directive_reason() {}
      // initialize test with non-default directive
      Test_State(unsigned n, bool t, Directive d, int i, std::string s) :
            expected(n), found(0), failed(0), has_plan(t), directive(d),
            directive_extent(i), directive_reason(s) {}

      unsigned expected;
      unsigned found;
      unsigned failed;
      Directive directive;
      int directive_extent;
      std::string directive_reason;
      bool has_plan;
    };

    struct no_plan_t {};
    struct skip_all_t {};
    struct todo_all_t {};

    constexpr std::stack<Test_State>::size_type subtest_limit = 3;
  }
  
  namespace {
    std::stack<details::Test_State> saved_tests;
    details::Test_State current_test;
  }

  const details::no_plan_t  no_plan  = details::no_plan_t();
  const details::skip_all_t skip_all = details::skip_all_t();
  const details::todo_all_t todo_all = details::todo_all_t();

  details::Directive get_directive() {
    return current_test.directive;
  }

  bool get_finished_testing() {
    return has_finished_testing;
  }

  bool _ok(bool condition, const std::string& description) {
    current_test.found++;

    if (current_test.directive == details::Directive::SKIP && current_test.directive_extent != 0) {
      *output << std::string(4 * saved_tests.size(), ' ') << "ok " << current_test.found << details::separator;
      *output << description << " # SKIP " << current_test.directive_reason << std::endl;

      current_test.directive_extent--;

      if (current_test.directive_extent == 0) {
        current_test.directive = details::Directive::NONE;
        current_test.directive_extent = -1;
      }
    }
    else if (current_test.directive == details::Directive::TODO && current_test.directive_extent != 0) {
      *output << std::string(4 * saved_tests.size(), ' ') << "not ok " << current_test.found << details::separator;
      *output << description << " # TODO " << current_test.directive_reason << std::endl;

      current_test.directive_extent--;

      if (current_test.directive_extent == 0) {
        current_test.directive = details::Directive::NONE;
        current_test.directive_extent = -1;
      }
    }
    else {
      *output << std::string(4 * saved_tests.size(), ' ') << (condition ? "ok " : "not ok ") << current_test.found; 
      *output << details::separator << description << std::endl;

      if (!condition) {
          current_test.failed++;
      }

      if (current_test.directive != details::Directive::NONE && current_test.directive_extent == 0) {
          current_test.directive = details::Directive::NONE;
          current_test.directive_extent = -1;
      }
    }

    return condition;
  }

  void plan(const details::no_plan_t& p, const std::string& name) {
    if (has_finished_testing) {
      throw fatal_exception(std::string("cannot plan after testing finished"));
    }

    if (current_test.has_plan && current_test.found > 0) {
      if (saved_tests.size() >= details::subtest_limit) {
        throw fatal_exception("subtest nesting exceeded limits");
      }
      else {
        saved_tests.push(current_test);

        current_test = details::Test_State(0, false,
                current_test.directive, -1, current_test.directive_reason);
      }
    }
    else {
      current_test.has_plan = true;
    }

    if (!name.empty()) {
      *output << std::string(4 * (saved_tests.size() - 1), ' ') << "**Test: " << name << "**" << std::endl;
    }
  }
  void plan(const details::skip_all_t& p, const std::string& name) {
    if (has_finished_testing) {
      throw fatal_exception(std::string("cannot plan after testing finished"));
    }

    if (current_test.has_plan && current_test.found > 0) {
      if (saved_tests.size() >= details::subtest_limit) {
        throw fatal_exception("subtest nesting exceeded limits");
      }
      else {
        if (current_test.directive == details::Directive::TODO) {
          throw fatal_exception(std::string("directive is TODO; cannot set it to SKIP"));
        }

        saved_tests.push(current_test);

        current_test = details::Test_State(0, true,
            current_test.directive, -1, current_test.directive_reason);
      }
    }
    else {
      current_test.has_plan = true;
    }

    if (!name.empty()) {
      *output << std::string(4 * (saved_tests.size() - 1), ' ') << "**Test: " << name << "**" << std::endl;
    }
  }
  void plan(const details::todo_all_t& p, const std::string& name) {
    if (has_finished_testing) {
      throw fatal_exception(std::string("cannot plan after testing finished"));
    }

    if (current_test.has_plan && current_test.found > 0) {
      if (saved_tests.size() >= details::subtest_limit) {
        throw fatal_exception("subtest nesting exceeded limits");
      }
      else {
        if (current_test.directive == details::Directive::SKIP) {
          throw fatal_exception(std::string("directive is SKIP; cannot set it to TODO"));
        }

        saved_tests.push(current_test);

        current_test = details::Test_State(0, true,
            current_test.directive, -1, current_test.directive_reason);
      }
    }
    else {
      current_test.has_plan = true;
    }

    if (!name.empty()) {
      *output << std::string(4 * (saved_tests.size() - 1), ' ') << "**Test: " << name << "**" << std::endl;
    }
  }
  void plan(unsigned tests, const std::string& name) {
    if (has_finished_testing) { 
      throw fatal_exception(std::string("cannot plan after testing finished"));
    }

    if (current_test.has_plan && current_test.found > 0) {
      if (saved_tests.size() >= details::subtest_limit) {
        throw fatal_exception(std::string("subtest nesting exceeded limits"));
      }
      else {
        saved_tests.push(current_test);

        current_test = details::Test_State(tests, true,
            current_test.directive, -1, current_test.directive_reason);
      }
    }
    else {
      current_test.has_plan = true;
      current_test.expected = tests;
    }

    if (!name.empty()) {
      *output << std::string(4 * (saved_tests.size() - 1), ' ') << "**Test: " << name << "**" << std::endl;
    }

    if (current_test.expected > 0) {
      *output << std::string(4 * saved_tests.size(), ' ') << "1.." << current_test.expected << std::endl;
    }
  }

  void done_testing() {
    if (has_finished_testing) {
      throw fatal_exception("done_testing() called too many times");
    }
    
    // plan computed retroactively when tests unknown or unplanned
    if (current_test.expected == 0 && !current_test.has_plan) {
      *output << std::string(4 * saved_tests.size(), ' ') << "1.." << current_test.found << std::endl;
    }
    
    if (current_test.found < current_test.expected && current_test.expected) {
      throw fatal_exception(std::string("ran too few tests: expected ")
            + std::to_string(current_test.expected)
            + std::string(", got ") + std::to_string(current_test.found));
    }

    if (current_test.found > current_test.expected && current_test.expected) {
       throw fatal_exception(std::string("ran too many tests: expected ")
            + std::to_string(current_test.expected)
            + std::string(", got ") + std::to_string(current_test.found));
    }

    if (saved_tests.empty()) {
      has_finished_testing = true;
      *output << std::endl;
    }
    else {
      /* 
       * because the outer test considers its subtest to be a single
       * test, the subtest's success is measured by the total number
       * of its failed tests. any failed tests cause the entire subtest
       * to be counted as a failure. this is the same reason that
       * directives are automatically inherited by subtests
       */
      bool subtest_failed = (current_test.failed > 0);

      current_test = saved_tests.top();
      current_test.found++;
      
      if (current_test.directive_extent > 0) {
        current_test.directive_extent--;
      }

      if (current_test.directive_extent == 0) {
        current_test.directive = details::Directive::NONE;
      }

      if (subtest_failed) {
        current_test.failed++;
      }

      saved_tests.pop();
    }
  }

  void done_testing(unsigned n) {
    if (has_finished_testing) {
      throw fatal_exception("done_testing() called too many times");
    }
    
    /* 
     * plan computed retroactively when tests unknown or unknown
     * argument n is used only if expecting 0 tests or no plan
     */
    if (current_test.expected == 0 && !current_test.has_plan) {
      if (current_test.found == n) { 
        *output << std::string(4 * saved_tests.size(), ' ') << "1.." << n << std::endl;
      }
      else {
        throw fatal_exception(std::string("found more tests than given to done_testing(unsigned)"));
      }
    }

    if (current_test.found < current_test.expected && current_test.expected) {
      throw fatal_exception(std::string("ran too few tests: expected ")
            + std::to_string(current_test.expected)
            + std::string(", got ") + std::to_string(current_test.found));
    }

    if (current_test.found > current_test.expected && current_test.expected) {
       throw fatal_exception(std::string("ran too many tests: expected ")
            + std::to_string(current_test.expected)
            + std::string(", got ") + std::to_string(current_test.found));
    }

    if (saved_tests.empty()) {
      has_finished_testing = true;
      *output << std::endl;
    }
    else {
      /* 
       * because the outer 'subtest' (I use this term liberally; note
       * that the implicit 'test' created by TAP when it is used may
       * also be called a subtest) considers its subtest to be a single
       * test, the subtest's success is measured by the total number
       * of its failed tests. any failed tests cause the entire subtest
       * to be counted as a failure. this is the same reason that
       * directives are automatically inherited by subtests
       */
      bool subtest_failed = (current_test.failed > 0);
      
      current_test = saved_tests.top();
      current_test.found++;

      if (current_test.directive_extent > 0) {
        current_test.directive_extent--;
      }

      if (current_test.directive_extent == 0) {
        current_test.directive = details::Directive::NONE;
      }

      if (subtest_failed) {
        current_test.failed++;
      }

      saved_tests.pop();
    }
  }

  unsigned planned() noexcept {
    return current_test.expected;
  }
  unsigned encountered() noexcept {
    return current_test.found;
  }

  int exit_status() {
    if (current_test.failed < 255) {
      return current_test.failed;
    }

    return 255;
  }

  bool summary() noexcept {
    return current_test.failed;
  }

  void bail_out(const std::string& reason) {
    *output << std::string(4 * saved_tests.size(), ' ') << "Bail out! " << reason << std::endl;

    std::exit(255); // does not unwind stack!
  }

  void pass(const std::string& message) {
    ok(true, message);
  }
  void fail(const std::string& message) {
    ok(false, message);
  }

  // mark the next num tests with skip directive
  void skip(unsigned num, const std::string& reason) {
    if (current_test.directive == details::Directive::TODO) {
      throw fatal_exception("cannot apply multiple directives simultaneously");
    }

    current_test.directive = details::Directive::SKIP;
    current_test.directive_extent = num;
    current_test.directive_reason = reason;
  }
  
  // mark the next test with skip directive
  void skip(const std::string& reason) {
    if (current_test.directive == details::Directive::TODO) {
      throw fatal_exception("cannot apply multiple directives simultaneously");
    }

    current_test.directive = details::Directive::SKIP;
    current_test.directive_extent = 1;
    current_test.directive_reason = reason;
  }
  
  // mark the next num tests with todo directive
  void todo(unsigned num, const std::string& reason) {
    if (current_test.directive == details::Directive::SKIP) {
      throw fatal_exception("cannot apply multiple directives simultaneously");
    }

    current_test.directive = details::Directive::TODO;
    current_test.directive_extent = num;
    current_test.directive_reason = reason;
  }
  
  // mark the next test with todo directive
  void todo(const std::string& reason) {
    if (current_test.directive == details::Directive::SKIP) {
      throw fatal_exception("cannot apply multiple directives simultaneously");
    }

    current_test.directive = details::Directive::TODO;
    current_test.directive_extent = 1;
    current_test.directive_reason = reason;
  }

  void set_yaml_pre_print(bool val) {
    pre_print_yaml = val;
  }

  void set_output(std::ostream& new_output) {
    if (!saved_tests.empty() && current_test.has_plan) {
      throw fatal_exception("Can't set output after plan()");
    }
    output = &new_output;
  }
  void set_error(std::ostream& new_error) {
    if (!saved_tests.empty() && current_test.has_plan) {
      throw fatal_exception("Can't set error after plan()");
    }
    error = &new_error;
  }
}
