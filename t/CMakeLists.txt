set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR})

add_executable (sanity "00-sanity.cpp")
target_link_libraries (sanity tap++)

add_executable (basic "10-basic.cpp")
target_link_libraries (basic tap++)

add_executable (extra "20-extras.cpp")
target_link_libraries (extra tap++)

set (CUSTOM_TEST_EXECUTABLES
  "${EXECUTABLE_OUTPUT_PATH}/sanity"
  "${EXECUTABLE_OUTPUT_PATH}/basic"
  "${EXECUTABLE_OUTPUT_PATH}/extra"
)

add_custom_target (run_tests
  COMMAND ${CUSTOM_TEST_DRIVER} ${CUSTOM_TEST_EXECUTABLES}
)
