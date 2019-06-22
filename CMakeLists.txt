cmake_minimum_required (VERSION 2.8)

# Configs
set (project_name ehp)
set (version_major 2)
set (version_minor 0)
set (version_patch 0)
set (config_default_www_root /www)
set (config_default_file /etc/ehp.conf)
set (config_default_log_file /var/log/ehp)

set (global_libraries m gc cord pthread)

project (${project_name})
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# Program config files
configure_file (
  "${PROJECT_SOURCE_DIR}/src/config.h.in"
  "${PROJECT_SOURCE_DIR}/src/config.h"
)

# Binary config files
configure_file (
  "${PROJECT_SOURCE_DIR}/config.in"
  "${PROJECT_SOURCE_DIR}/config"
)

# Debug & Release building
if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Test")
    aux_source_directory (src src_objs)
    add_executable (${project_name} ${src_objs})
    target_link_libraries (${project_name} ${global_libraries})
endif()

# Tests building
if(${CMAKE_BUILD_TYPE} STREQUAL "Test")
    enable_testing()
    add_subdirectory ("${PROJECT_SOURCE_DIR}/test")
endif()
