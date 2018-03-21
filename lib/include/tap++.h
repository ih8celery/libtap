#ifndef LIB_TAPPP_TAPPP_H
#define LIB_TAPPP_TAPPP_H

#include <iostream>
#include <string>
#include <type_traits>
#include <cmath>

#define FLOAT_CMP(left, right, e) (2 * fabs(double(left) - double(right)) / (fabs(double(left)) + fabs(double(right))) > (e))

#define ok(condition, description) do {\
  if (TAP::get_finished_testing()) {\
    throw fatal_exception(std::string("testing has finished; no new tests allowed"));\
  }\
  if (TAP::get_directive() == TAP::details::Directive::NONE) {\
    TAP::details::test_status = TAP::_ok((condition), (description));\
  }\
  else if (TAP::get_directive() == TAP::details::Directive::SKIP) {\
    TAP::details::test_status = TAP::_ok(true, (description));\
  }\
  else {\
    TAP::details::test_status = TAP::_ok(false, (description));\
  }\
} while (0)

#define not_ok(condition, description) do {\
  if (TAP::get_finished_testing()) {\
    throw fatal_exception(std::string("testing has finished; no new tests allowed"));\
  }\
  if (TAP::get_directive() == TAP::details::Directive::NONE) {\
    TAP::details::test_status = TAP::_ok(!(condition), (description));\
  }\
  else if (TAP::get_directive() == TAP::details::Directive::SKIP) {\
    TAP::details::test_status = TAP::_ok(true, (description));\
  }\
  else {\
    TAP::details::test_status = TAP::_ok(false, (description));\
  }\
} while (0)

namespace TAP {
  namespace details {
    enum class Directive { NONE, SKIP, TODO };

    struct Test_State;

    struct no_plan_t;
    struct skip_all_t;
    struct todo_all_t;

    extern bool test_status;
    extern const char * separator;
  }

  class fatal_exception : public std::exception {
    std::string msg;

    public:
      fatal_exception(std::string _msg) : msg(_msg) {}
      ~fatal_exception() = default;

      const char * what() const noexcept override { return msg.c_str(); }
  };

  namespace {
    std::ostream* output = &std::cout;
    std::ostream* error = &std::cerr;

    std::string test_indent;

    bool has_finished_testing = false;

    bool pre_print_yaml = false;
  }

  extern const details::no_plan_t no_plan;
  extern const details::skip_all_t skip_all;
  extern const details::todo_all_t todo_all;

  details::Directive get_directive();

  bool get_finished_testing();

  bool _ok(bool, const std::string& = "");

  void plan(const details::no_plan_t&, const std::string& = "");
  void plan(const details::skip_all_t&, const std::string& = "");
  void plan(const details::todo_all_t&, const std::string& = "");
  void plan(unsigned, const std::string& = "");

  void done_testing();
  void done_testing(unsigned);

  unsigned planned() noexcept;
  unsigned encountered() noexcept;

  void pass(const std::string& = "");
  void fail(const std::string& = "");

  void skip(unsigned, const std::string& = "");
  void skip(const std::string& = "");

  void todo(unsigned, const std::string& = "");
  void todo(const std::string& = "");

  void bail_out(const std::string& reason);

  int exit_status();
  bool summary() noexcept;

  void set_yaml_pre_print(bool);
  void set_output(std::ostream&);
  void set_error(std::ostream&); 
  
  // diagnostic templates {
  template<typename T>
  void diag(const T& first) {
    *error << "# " << first << std::endl;
  }
  template<typename T1, typename T2>
  void diag(const T1& first, const T2& second) {
    *error << "# " << first << second << std::endl;
  }
  template<typename T1, typename T2, typename T3>
  void diag(const T1& first, const T2& second, const T3& third) {
    *error << "# " << first << second << third << std::endl;
  }
  template<typename T1, typename T2, typename T3, typename T4>
  void diag(const T1& first, const T2& second, const T3& third, const T4& fourth) {
    *error << "# " << first << second << third << fourth << std::endl;
  }
  template<typename T1, typename T2, typename T3, typename T4, typename T5>
  void diag(const T1& first, const T2& second, const T3& third, const T4& fourth, const T5& fifth) {
    *error << "# " << first << second << third << fourth << fifth << std::endl;
  }
  template <typename T, typename U>
  void yaml_fail(const std::string& msg, const T& got, const U& expect) {
    *error << test_indent << "  ---" << std::endl;
    *error << test_indent << "  message: " << msg << std::endl;
    *error << test_indent << "  severity: fail" << std::endl;
    *error << test_indent << "  data:" << std::endl;
    *error << test_indent << "    got: " << got << std::endl;
    *error << test_indent << "    expect: " << expect << std::endl;
    *error << test_indent << "  ..." << std::endl;
  }
  
  // }
  // comment templates {
  template<typename T>
  void note(const T& first) {
    *output << "# " << first << std::endl;
  }
  template<typename T1, typename T2>
  void note(const T1& first, const T2& second) {
    *output << "# " << first << second << std::endl;
  }
  template<typename T1, typename T2, typename T3>
  void note(const T1& first, const T2& second, const T3& third) {
    *output << "# " << first << second << third << std::endl;
  }
  template<typename T1, typename T2, typename T3, typename T4> 
  void note(const T1& first, const T2& second, const T3& third, const T4& fourth) {
    *output << "# " << first << second << third << fourth << std::endl;
  }
  template<typename T1, typename T2, typename T3, typename T4, typename T5> 
  void note(const T1& first, const T2& second, const T3& third, const T4& fourth, const T5& fifth) {
    *output << "# " << first << second << third << fourth << fifth << std::endl;
  }
  // }

  // is_* family of templates {
  template<typename T, typename U> 
  typename std::enable_if<!std::is_floating_point<U>::value, void>::type 
  is(const T& left, const U& right, const std::string& description = "") {
    bool cmp_failed = !(left == right);

    if (pre_print_yaml && cmp_failed) {
      yaml_fail(description, left, right);
    }

    not_ok(cmp_failed, description);

    if (!pre_print_yaml && cmp_failed) {
      yaml_fail(description, left, right);
    }
  }

  template<typename T, typename U>
  typename std::enable_if<!std::is_floating_point<U>::value, void>::type
  isnt(const T& left, const U& right, const std::string& description = "") {
    bool cmp_success = (left == right);

    if (pre_print_yaml && cmp_success) {
      yaml_fail(description, left, right);
    }

    not_ok(cmp_success, description);

    if (!pre_print_yaml && cmp_success) {
      yaml_fail(description, left, right);
    }
  }

  template<typename T, typename U>
  typename std::enable_if<std::is_floating_point<U>::value, void>::type
  is(const T& left, const U& right, const std::string& description = "", double epsilon = 0.01) {
    bool cmp_failed = !(FLOAT_CMP(left, right, epsilon));

    if (pre_print_yaml && cmp_failed) {
      yaml_fail(description, left, right);
    }

    not_ok(cmp_failed, description);

    if (!pre_print_yaml && cmp_failed) {
      yaml_fail(description, left, right);
    }
  }

  template<typename T, typename U>
  typename std::enable_if<std::is_floating_point<U>::value, void>::type
  isnt(const T& left, const U& right, const std::string& message = "", double epsilon = 0.01) {
    bool cmp_success = FLOAT_CMP(left, right, epsilon);

    if (pre_print_yaml && cmp_success) {
      yaml_fail(message, left, right);
    }

    not_ok(cmp_success, message);

    if (!pre_print_yaml && cmp_success) {
      yaml_fail(message, left, right);
    }
  }

  template<typename T, typename U> void is_convertible(const std::string& description) {
    _ok(std::is_convertible<T, U>::value, description);
  }

  template<typename T, typename U> void isnt_convertible(const std::string& description) {
    _ok(!std::is_convertible<T, U>::value, description);
  }
}

#ifdef WANT_TEST_EXTRAS
/* 
 * exception-handling analogue to TAP::ok, where code that throws an
 * exception fails the test
 */
#define TRY_OK(code, description) do {\
    try {\
      code;\
      TAP::pass(description);\
    }\
    catch (const std::exception& e) {\
      TAP::fail(description);\
      TAP::note("Caught exception: ", e.what());\
    }\
    catch (...) {\
      TAP::fail(description);\
    }\
  } while (0)

/*
 * exception-handling analogue to TAP::not_ok, where code that does
 * NOT throw an exception fails the test
 */
#define TRY_NOT_OK(code, description) do {\
    try {\
      code;\
      TAP::fail(description);\
      TAP::note("Expected an exception");\
    }\
    catch (const std::exception& e) {\
      TAP::pass(description);\
      TAP::note("Caught exception: ", e.what());\
    }\
    catch (...) {\
      TAP::pass(description);\
      TAP::note("Caught non-standard exception");\
    }\
  } while (0)

/* 
 * begin a sequence of tests enclosed in a try/catch block.
 * paired with TEST_END
 */
#define TEST_START(num) {\
    const char* _current_message = NULL;\
    TAP::plan(num);\
    try {

/*
 * end a sequence of tests by checking that the planned number of tests
 * were run and by handling TAP-specific and standard exceptions.
 * paired with TEST_START
 */
#define TEST_END \
      if (TAP::encountered() < TAP::planned()) {\
        TAP::note("Looks like you planned ", TAP::planned(), " tests but only ran ", TAP::encountered());\
      }\
      else if(TAP::encountered() > TAP::planned()) {\
        TAP::note("Looks like you planned ", TAP::planned(), " tests but ran ", TAP::encountered() - TAP::planned(), " extra");\
      }\
    }\
    catch(TAP::details::Skip_exception& skipper) {\
      TAP::skip(TAP::planned() - TAP::encountered(), skipper.reason);\
    }\
    catch(TAP::details::Todo_exception& todoer) {\
      /*TODO*/\
    }\
    catch(const TAP::fatal_exception& e) {\
      if(_current_message) TAP::fail(_current_message);\
      TAP::note("A fatal error occured:,", e.what()," aborting");\
    }\
    catch(const std::exception& e) {\
      if(_current_message) TAP::fail(_current_message);\
      TAP::note("Got unknown error: ", e.what());\
    }\
    catch (...) {\
      if(_current_message) TAP::fail(_current_message);\
    }\
    return TAP::exit_status();\
  }

/* declare named group of tests */
#define SUBTEST(planned, name) \
  TAP::plan(planned, name);

/* 
 * start anonymous group of tests or continue current group if it has
 * not yet run any tests 
 */
#define DO \
  TAP::plan(TAP::no_plan);\
  try {

/* conclude a group of tests begun by DO */
#define END \
    TAP::done_testing();\
  }\
  catch (const TAP::fatal_exception& e) {\
    TAP::diag("TAP error found while running subtest");\
  }

/* This small macro is a main reason for this ugly exercise. I can't introduce a new scope because
 * code subsequent to the declaration should be visible to the rest of the code. At the same time, it
 * must be exception safe. These are quite severe constraints :-(.
 */
#define TRY_DECL(action, new_message) \
  _current_message = new_message;\
  action;\
  TAP::pass(_current_message);\
  _current_message = NULL

#endif /*WANT_TEST_EXTRAS*/
// }

#endif /*LIB_TAPPP_TAPPP_H*/
