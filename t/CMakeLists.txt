set (TEST_DIR ${CMAKE_CURRENT_LIST_DIR})

add_executable (sanity "00-sanity.cpp")
target_link_libraries (sanity tap++)

add_executable (basic "10-basic.cpp")
target_link_libraries (basic tap++)

add_executable (extra "20-extras.cpp")
target_link_libraries (extra tap++)

add_custom_target (run_tests
  COMMAND "${TEST_DIR}/sanity"
  COMMAND "${TEST_DIR}/basic"
  COMMAND "${TEST_DIR}/extra"
)
