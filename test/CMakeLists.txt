cmake_minimum_required (VERSION 2.8)

enable_testing()
find_package(GTest REQUIRED)
include_directories(../src)
include_directories(${GTEST_INCLUDE_DIR})
set (test_objs ../src/array.cc;../src/str.cc;../src/map.cc)

macro (new_test test_name)
    set (test_exec "test-${test_name}")
    add_executable(${test_exec} "${test_name}.cc" ${test_objs})
    target_link_libraries(${test_exec} ${GTEST_BOTH_LIBRARIES} ${global_libraries})
    add_test (${test_name} ${test_exec})
endmacro (new_test)

new_test(str)
new_test(array)
new_test(map)
