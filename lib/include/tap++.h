#ifndef LIB_TAPPP_TAPPP_H
#define LIB_TAPPP_TAPPP_H

#include <iostream>
#include <string>
#include <type_traits>
#include <cmath>

namespace TAP {
  namespace details {
    struct skip_all_type {};
    struct no_plan_type {};

    extern std::ostream* output;
    extern std::ostream* error;

    // Return the variant of "Failed test" or "Failed
    // (TODO) test" required by whether the current test is
    // a todo test
    char const * failed_test_msg() throw();
  }
  class fatal_exception : public std::exception {
    std::string message;

    public:
    fatal_exception(const std::string& _message) : message(_message) {
    }
    const char* what() const noexcept override {
      return message.c_str();
    }
    ~fatal_exception() noexcept override {}
  };
  
  extern const details::skip_all_type skip_all;
  extern const details::no_plan_type no_plan;

  void plan(unsigned);
  void plan(const details::skip_all_type&, const std::string& = "");
  void plan(const details::no_plan_type&) noexcept;

  void done_testing();
  void done_testing(unsigned);

  unsigned planned() noexcept;
  unsigned encountered() noexcept;

  bool ok(bool, const std::string& = "");
  bool not_ok(bool, const std::string& = "");

  bool pass(const std::string& = "");
  bool fail(const std::string& = "");

  void skip(unsigned, const std::string& = "");
  void bail_out(const std::string& reason);

  int exit_status();
  bool summary() noexcept;

  void set_output(std::ostream&);
  void set_error(std::ostream&); 

  template<typename T>
  void diag(const T& first) {
    *details::error << "# " << first << std::endl;
  }
  template<typename T1, typename T2>
  void diag(const T1& first, const T2& second) {
    *details::error << "# " << first << second << std::endl;
  }
  template<typename T1, typename T2, typename T3>
  void diag(const T1& first, const T2& second, const T3& third) {
    *details::error << "# " << first << second << third << std::endl;
  }
  template<typename T1, typename T2, typename T3, typename T4>
  void diag(const T1& first, const T2& second, const T3& third, const T4& fourth) {
    *details::error << "# " << first << second << third << fourth << std::endl;
  }
  template<typename T1, typename T2, typename T3, typename T4, typename T5>
  void diag(const T1& first, const T2& second, const T3& third, const T4& fourth, const T5& fifth) {
    *details::error << "# " << first << second << third << fourth << fifth << std::endl;
  }

  template<typename T>
  void note(const T& first) {
    *details::output << "# " << first << std::endl;
  }
  template<typename T1, typename T2>
  void note(const T1& first, const T2& second) {
    *details::output << "# " << first << second << std::endl;
  }
  template<typename T1, typename T2, typename T3>
  void note(const T1& first, const T2& second, const T3& third) {
    *details::output << "# " << first << second << third << std::endl;
  }
  template<typename T1, typename T2, typename T3, typename T4> 
  void note(const T1& first, const T2& second, const T3& third, const T4& fourth) {
    *details::output << "# " << first << second << third << fourth << std::endl;
  }
  template<typename T1, typename T2, typename T3, typename T4, typename T5> 
  void note(const T1& first, const T2& second, const T3& third, const T4& fourth, const T5& fifth) {
    *details::output << "# " << first << second << third << fourth << fifth << std::endl;
  }

  template<typename T, typename U> 
  typename std::enable_if<!std::is_floating_point<U>::value, bool>::type 
  is(const T& left, const U& right, const std::string& description = "") {
    using namespace TAP::details;

    try {
      bool is_ok = ok(left == right, description);

      if (!is_ok) { // TODO is this messaging required by TAP13? if so, apply to below as well
        diag(failed_test_msg()," '", description, "'");
        diag("       Got: ", left);
        diag("  Expected: ", right);
      }

      return is_ok;
    }
    catch(const std::exception& e) {
      fail(description);
      diag(failed_test_msg()," '", description, "'");
      diag("Caught exception '", e.what(), "'");
      diag("       Got: ", left);
      diag("  Expected: ", right);

      return false;
    }
    catch(...) {
      fail(description);
      diag(failed_test_msg()," '", description, "'");
      diag("Caught unknown exception");
      diag("       Got: ", left);
      diag("  Expected: ", right);

      return false;
    }
  }

  template<typename T, typename U>
  typename std::enable_if<!std::is_floating_point<U>::value, bool>::type
  isnt(const T& left, const U& right, const std::string& description = "") {
    try {
      return ok(left != right, description);
    }
    catch(const std::exception& e) {
      fail(description);
      diag("In test ", description);
      diag("Caught exception: ", e.what());

      return false;
    }
    catch(...) {
      fail(description);
      diag("In test ", description);
      diag("Caught unknown exception");

      return false;
    }
  }

  template<typename T, typename U>
  typename std::enable_if<std::is_floating_point<U>::value, bool>::type
  is(const T& left, const U& right, const std::string& description = "", double epsilon = 0.01) {
    using namespace TAP::details;

    try {
      bool is_ok = ok(2 * fabs(left - right) / (fabs(left) + fabs(right)) < epsilon);

      if (!is_ok) {
        diag(failed_test_msg()," '", description, "'");
        diag("       Got: ", left);
        diag("  Expected: ", right);
      }

      return is_ok;
    }
    catch(const std::exception& e) {
      fail(description);
      diag(failed_test_msg()," '", description, "'");
      diag("Caught exception '", e.what(), "'");
      diag("       Got: ", left);
      diag("  Expected: ", right);

      return false;
    }
    catch(...) {
      fail(description);
      diag(failed_test_msg()," '", description, "'");
      diag("Caught unknown exception");
      diag("       Got: ", left);
      diag("  Expected: ", right);

      return false;
    }
  }

  template<typename T, typename U>
  typename std::enable_if<std::is_floating_point<U>::value, bool>::type
  isnt(const T& left, const U& right, const std::string& message = "", double epsilon = 0.01) {
    using namespace TAP::details;

    try {
      bool is_greater = 2 * fabs(left - right) / (fabs(left) + fabs(right)) > epsilon;
      
      return ok(is_greater, message);
    }
    catch(const std::exception& e) {
      fail(message);
      diag(failed_test_msg()," '", message, "'");
      diag("Caught exception '", e.what(), "'");

      return false;
    }
    catch(...) {
      fail(message);
      diag(failed_test_msg()," '", message, "'");
      diag("Caught unknown exception");

      return false;
    }
  }

  /* test whether type T may be converted to U */
  template<typename T, typename U> bool is_convertible(const std::string& description) {
    return ok(std::is_convertible<T, U>::value, description);
  }

  /* deprecated. use isnt_convertible instead */
  template<typename T, typename U> bool is_inconvertible(const std::string& description) {
    return ok(!std::is_convertible<T, U>::value, description);
  }

  /* test whether type T may NOT be converted to U */
  template<typename T, typename U> bool isnt_convertible(const std::string& description) {
    return ok(!std::is_convertible<T, U>::value, description);
  }

  extern std::string TODO; 

  class todo_guard {
    const std::string value;

  public:
    todo_guard() noexcept;
    ~todo_guard() noexcept;
  };
}

#ifdef WANT_TEST_EXTRAS

namespace TAP {
  namespace details {
    struct Skip_exception {
      const std::string reason;
      Skip_exception(const std::string& _reason) noexcept : reason(_reason) {}
    };

    struct Todo_exception {
      const std::string reason;
      Todo_exception(const std::string& _reason) noexcept : reason(_reason) {}
    };

    void start_block(unsigned) noexcept;
    unsigned stop_block();

  }

  void skip(const std::string& reason);
  void skip_todo(const std::string& reason);
}

/* deprecated. use TRY_OK instead */
#define TRY(action, name) do {\
    try {\
      action;\
      TAP::pass(name);\
    }\
    catch (const std::exception& e) {\
      TAP::fail(name);\
      TAP::note("Caught exception: ", e.what());\
    }\
    catch (...) {\
      TAP::fail(name);\
    }\
  } while (0)

/* 
 * exception-handling analogue to TAP::ok, where code that throws an
 * exception fails the test
 * 
 * NOTE: creates a new scope for try/catch block
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
 * 
 * NOTE: creates a new scope for try/catch block
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

/* deprecated. use TRY_NOT_OK instead */
#define FAIL(action, name) do {\
    try {\
      action;\
      TAP::fail(name);\
    }\
    catch (...) {\
      TAP::pass(name);\
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

/*
 * TODO document
 */
#define BLOCK_START(planned) \
  try {\
    todo_guard foo##planned;\
    TAP::details::start_block(planned);

/*
 * TODO document
 */
#define BLOCK_END \
    if (TAP::encountered() != TAP::details::stop_block()) {\
      TODO::note("Not enough tests for plan!");\
    }\
  }\
  catch(TAP::details::Skip_exception& skipper) {\
    TAP::skip(TAP::details::stop_block() - TAP::encountered(), skipper.reason);\
  }\
  catch(TAP::details::Todo_exception& todoer) {\
    TODO::note("Todo tests not implemented yet");\
    /*TODO*/\
  }\
  catch(const std::exception& e) {\
    TAP::fail(_current_message);\
    TODO::note("Got error: ", e.what());\
  }\
  catch (...) {\
    TAP::fail(_current_message);\
    TODO::note("Died with some mysterious error");\
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
