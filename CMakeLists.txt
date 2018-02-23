cmake_minimum_required (VERSION 3.0)

project (TAP++ LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)
set (CUSTOM_TEST_DRIVER "${CMAKE_CURRENT_LIST_DIR}/lib/tap")
set (HEADER_DIR "${PROJECT_SOURCE_DIR}/lib/include")

enable_testing ()

include_directories (${HEADER_DIR})

add_subdirectory (lib/src)
add_subdirectory (t)

install (FILES "${HEADER_DIR}/tap++.h" DESTINATION include)
