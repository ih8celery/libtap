/**
 * \file tap++.h
 *
 * \author Adam Marshall (ih8celery)
 *
 * \brief declare interface functions and accessible global variables 
 */

#ifndef LIB_TAPPP_TAPPP_H
#define LIB_TAPPP_TAPPP_H

#include <iostream>
#include <string>
#include <type_traits>
#include <cmath>

/**
 * unnecessary macro function
 */
#define FLOAT_CMP(left, right, e) (2 * fabs(double(left) - double(right)) / (fabs(double(left)) + fabs(double(right))) > (e))

/**
 * \define ok(condition, description)
 *
 * \brief primary testing function. uses 'condition' to determine<br>
 * state of the test, which is described by argument 'description'.<br>
 * because this is a macro, it is able to avoid evaluation of its<br>
 * first argument when certain directives are in effect
 */
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

/**
 * \define not_ok(condition, description)
 * 
 * \brief similar to ok, but has the opposite treatment of 'condition'
 */
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
    /**
     * \enum Directive
     *
     * \brief defines the three directives controlling the test<br>
     * emitter. NONE is the default directive; tests proceed normally.<br>
     * SKIP causes tests to be skipped; in most cases, the condition<br>
     * will not be evaluated. TODO marks tests as unfinished; tests<br>
     * marked this way also do not evaluate their conditions
     */
    enum class Directive { NONE, SKIP, TODO };

    /**
     * \struct Test_State
     *
     * \brief contains information about test groups which is saved<br>
     * on a stack
     */
    struct Test_State;

    /**
     * \struct no_plan_t
     * \brief used to create a constant passed to plan function.<br>
     * a test group with no plan will print its 'header' running<br>
     * its tests
     */
    struct no_plan_t;

    /**
     * \struct skip_all_t
     * \brief used to create a constant passed to plan function.<br>
     * a test group marked thus will not fully evaluate its tests
     */
    struct skip_all_t;

    /** 
     * \struct todo_all_t
     * \brief used to create a constant passed to plan function.<br>
     * all tests in such a test group are marked 'todo'
     */
    struct todo_all_t;

    /**
     * \var bool test_status
     * \brief success state of last run test
     */
    extern bool test_status;

    /**
     * \var const char * separator
     * \brief short string separating test state from description in output
     */
    extern const char * separator;
  }

  /**
   * \class fatal_exception
   * \brief encapsulate errors in testing from which TAP cannot recover
   */
  class fatal_exception : public std::exception {
    std::string msg;

    public:
      fatal_exception(std::string _msg) : msg(_msg) {}
      ~fatal_exception() = default;

      const char * what() const noexcept override { return msg.c_str(); }
  };

  namespace {
    /**
     * \var std::ostream* output
     * \brief stream TAP prints normal output to
     */
    std::ostream* output = &std::cout;

    /**
     * \var std::ostream* error
     * \brief stream TAP prints errors to
     */
    std::ostream* error = &std::cerr;

    /**
     * \var bool has_finished_testing
     * \brief false until the outermost test group is finished
     */
    bool has_finished_testing;

    std::string test_indent;

    bool pre_print_yaml = false;
  }

  /**
   * \var const no_plan_t no_plan
   */
  extern const details::no_plan_t no_plan;

  /**
   * \var const skip_all_t skil_all
   */
  extern const details::skip_all_t skip_all;

  /**
   * \var const todo_all_t todo_all
   */
  extern const details::todo_all_t todo_all;

  /**
   * \fn Directive get_directive()
   * \brief return the value of the current directive
   */
  details::Directive get_directive();

  /**
   * \fn bool get_finished_testing()
   * \brief return the value of has_finished_testing
   */
  bool get_finished_testing();

  /**
   * \fn bool _ok(bool, const string& = "")
   * \brief auxiliary function to most tests
   */
  bool _ok(bool, const std::string& = "");

  /**
   * \fn void plan(const no_plan_t&, const string& = "")
   * \brief explicitly state that test group has no plan
   */
  void plan(const details::no_plan_t&, const std::string& = "");
  
  /**
   * \fn void plan(const skip_all_t&, const string&= "")
   * \brief explicitly state that all tests in group should be skipped
   */
  void plan(const details::skip_all_t&, const std::string& = "");
  
  /**
   * \fn void plan(const todo_all_t&, const string& = "")
   * \brief explicitly state that all tests in group are unfinished
   */
  void plan(const details::todo_all_t&, const std::string& = "");
  
  /**
   * \fn void plan(unsigned, const string& = "")
   * \brief specify that test group contains a certain number of tests
   */
  void plan(unsigned, const std::string& = "");

  /**
   * \fn void done_testing()
   * \brief declare that testing the current group has finished
   */
  void done_testing();
  
  /**
   * \fn void done_testing(unsigned)
   * \brief declare that testing a group has finished after running<br>
   * a number of tests. to be used when the current test group was<br>
   * unplanned to give late a number of expected tests
   */
  void done_testing(unsigned);

  /**
   * \fn unsigned planned()
   * \brief return number of tests planned for this group
   */
  unsigned planned() noexcept;

  /**
   * \fn unsigned encountered()
   * \brief return number of tests found so far in a group
   */
  unsigned encountered() noexcept;

  /**
   * \fn void pass(const string& = "")
   * \brief automatically pass test described by string
   */
  void pass(const std::string& = "");
  
  /**
   * \fn void pass(const string& = "")
   * \brief automatically fail test described by string
   */
  void fail(const std::string& = "");

  /**
   * \fn void fail(const string& = "")
   * \brief skip a certain number of tests for reason in string
   */
  void skip(unsigned, const std::string& = "");

  /**
   * \fn void skip(const string& = "")
   * \brief skip the next test for reason given in string
   */
  void skip(const std::string& = "");

  /**
   * \fn void todo(unsigned, const string& = "")
   * \brief mark a certain number of tests unfinished for reason<br>
   * given by string
   */
  void todo(unsigned, const std::string& = "");
  
  /**
   * \fn void todo(const string& = "")
   * \brief mark next test unfinished for reason given by string
   */
  void todo(const std::string& = "");

  /**
   * \fn void bail_out(const string& = "")
   * \brief immediately exit from the test process
   */
  void bail_out(const std::string& reason);

  /**
   * \fn int exit_status()
   * \brief return the number of tests failed or 255, whichever is smaller
   */
  int exit_status();
  
  /**
   * \fn bool summary()
   * \brief return success or failure of test group
   */
  bool summary() noexcept;

  /**
   * \fn void set_yaml_pre_print(bool)
   * \brief specify the order of printing of yaml diagnostics with<br>
   * respect to main test output
   */
  void set_yaml_pre_print(bool);
  
  /**
   * \fn void set_output(ostream&)
   * \brief set the output stream
   */
  void set_output(std::ostream&);
  
  /**
   * \fn void set_error(ostream&)
   * \brief set the error stream
   */
  void set_error(std::ostream&); 
  
  // diagnostic templates
  /**
   * \fn template<class T> void diag(const T&)
   * \brief print diagnostic message to error stream
   */
  template<typename T>
  void diag(const T& first) {
    *error << "# " << first << std::endl;
  }
  
  /**
   * \fn template<class T1, class T2> void diag(const T1&, const T2&)
   */
  template<typename T1, typename T2>
  void diag(const T1& first, const T2& second) {
    *error << "# " << first << second << std::endl;
  }
  
  /**
   * \fn template<class T,class U,class V> void diag(const T&,const U&,const V&)
   */
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

  /**
   * \fn template<class T,class U> void yaml_fail(const string&, const T&, const U&)
   * \brief print diagnostic as inline yaml document upon failure of 'is' or 'isnt'
   */
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
  
  // comment templates
  /**
   * \fn template<class T> void note(const T&)
   * \brief print ancillary information to output stream
   */
  template<typename T>
  void note(const T& first) {
    *output << "# " << first << std::endl;
  }

  /**
   * \fn template<class T,class U> void note(const T&,const U&)
   */
  template<typename T1, typename T2>
  void note(const T1& first, const T2& second) {
    *output << "# " << first << second << std::endl;
  }

  /**
   * \fn template<class T,class U,class V> void note(const T&,const U&,const V&)
   */
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

  // is_* family of templates
  /**
   * \fn template<class T, class U> void is(const T&, const U&, const string& = "")
   * \brief compare first argument to second. if the two are "==", the<br>
   * test succeeds. this template will not be selected if second arg<br>
   * is a floating point type
   */
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
  
  /**
   * \fn template<class T, class U> void isnt(const T&, const U&, const string& = "")
   * \brief compare first argument to second. if the two are NOT "==", the<br>
   * test succeeds. this template will not be selected if second arg<br>
   * is a floating point type
   */
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
  
  /**
   * \fn template<class T, class U> void is(const T&, const U&, const string& = "", double = 0.01)
   * \brief compare first argument to second. if the two are "==", the<br>
   * test succeeds. IMPORTANT: this template will be selected only if<br>
   * its second argument is a floating point. Float arguments are allowed<br>
   * to differ by an epsilon with a default value of 0.01
   */
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

  /**
   * \fn template<class T, class U> void isnt(const T&, const U&, const string& = "", double = 0.01)
   * \brief compare first argument to second. if the two are NOT "==", the<br>
   * test succeeds. IMPORTANT: this template will only be selected if<br>
   * second its second argument is a floating point. Float arguments<br>
   * are allowed to differ by an epsilon with a default value of 0.01
   */
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

  /**
   * \fn template<class T, class U> void is_convertible(const string&)
   * \brief test whether type T can be converted to U
   */
  template<typename T, typename U> void is_convertible(const std::string& description) {
    _ok(std::is_convertible<T, U>::value, description);
  }

  /**
   * \fn template<class T, class U> void isnt_convertible(const string&)
   * \brief test whether type T can NOT be converted to U
   */
  template<typename T, typename U> void isnt_convertible(const std::string& description) {
    _ok(!std::is_convertible<T, U>::value, description);
  }
}

#ifdef WANT_TEST_EXTRAS
/**
 * \define TRY_OK(code, description)
 * \brief exception-handling analogue to TAP::ok, where code that throws<br> 
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

/**
 * \define TRY_NOT_OK(code, description)
 * \brief exception-handling analogue to TAP::not_ok, where code that<br>
 * does NOT throw an exception fails the test
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

/**
 * \define TEST_START(plan)
 * \brief begin a sequence of tests enclosed in a try/catch block<br>
 * to capture test related exceptions. paired with TEST_END
 */
#define TEST_START(plan) {\
  const char* _current_message = NULL;\
  TAP::plan(plan);\
  try {

/**
 * \define TEST_END
 * \brief end a sequence of tests by checking that the planned number<br>
 * of tests were run and by handling TAP-specific and standard exceptions.<br>
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

/**
 * \define SUBTEST(planned, name)
 * \brief declare named group of tests with a plan
 */
#define SUBTEST(planned, name) \
  TAP::plan(planned, name);

/** 
 * \define DO
 * \brief start anonymous group of tests or continue current group if<br>
 * it has not yet run any tests 
 */
#define DO \
  TAP::plan(TAP::no_plan);\
  try {

/**
 * \define END
 * \brief conclude a group of tests begun by DO
 */
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

#endif /*LIB_TAPPP_TAPPP_H*/
