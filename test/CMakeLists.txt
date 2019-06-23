cmake_minimum_required (VERSION 2.8)

enable_testing()
find_package(GTest REQUIRED)
include_directories(../src)
include_directories(${GTEST_INCLUDE_DIR})
set (test_objs ../src/args.cc;../src/array.cc;../src/config.cc;../src/file.cc;../src/http.cc;../src/http_constant.cc;../src/lock.cc;../src/log.cc;../src/map.cc;../src/net.cc;../src/poller.cc;../src/process.cc;../src/psignal.cc;../src/ptime.cc;../src/ptimer.cc;../src/regexp.cc;../src/str.cc;../src/url.cc)

macro (new_test test_name)
    set (test_exec "test-${test_name}")
    add_executable(${test_exec} "${test_name}.cc" ${test_objs})
    target_link_libraries(${test_exec} ${GTEST_BOTH_LIBRARIES} ${global_libraries})
    add_test (${test_name} ${test_exec})
endmacro (new_test)

new_test(str)
new_test(array)
new_test(map)
