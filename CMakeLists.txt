cmake_minimum_required (VERSION 3.0)

project (TAP++ LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)

include_directories ("${PROJECT_SOURCE_DIR}/lib/include")

add_subdirectory (lib/src)

add_subdirectory (t)
