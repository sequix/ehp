cmake_minimum_required (VERSION 2.8)

# configs
set (project_name ehp)
set (version_major 1)
set (version_minor 0)
set (version_patch 0)
set (config_www_root root)
set (config_log_file ehp.log)
set (config_default_log_file ehp.log)
set (pconfig_hash_seed 42)

set (global_libraries m gc cord)

project (${project_name})
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# config files
configure_file (
  "${PROJECT_SOURCE_DIR}/src/config.h.in"
  "${PROJECT_SOURCE_DIR}/src/config.h"
)

configure_file (
  "${PROJECT_SOURCE_DIR}/config-example.in"
  "${PROJECT_SOURCE_DIR}/config-example"
)

# Debug & Release building
if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Test")
    # source files
    aux_source_directory (src src_objs)
    add_executable (${project_name} ${src_objs})
    target_link_libraries (${project_name} ${global_libraries})

    # external package - argtable3
    add_subdirectory ("${PROJECT_SOURCE_DIR}/pkg/argtable3")
    include_directories ("${PROJECT_SOURCE_DIR}/pkg/argtable3")
    target_link_libraries (${project_name} argtable3)
endif()

# Tests building
if(${CMAKE_BUILD_TYPE} STREQUAL "Test")
    enable_testing()
    add_subdirectory ("${PROJECT_SOURCE_DIR}/test")
endif()
