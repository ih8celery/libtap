set (LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR})

set (CMAKE_CXX_FLAGS "-fpermissive")

add_library (tap++ SHARED tap++.cpp)

install (TARGETS tap++ DESTINATION lib)
