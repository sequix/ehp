cmake_minimum_required (VERSION 2.8)

# configs
set (project_name ehp)
set (version_major 0)
set (version_minor 1)
set (version_patch 0)

set (www_root root)

set (log_file ehp.log)
set (default_log_file ehp.log)

project (${project_name})
add_compile_options(-std=c99)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
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

# source files
aux_source_directory (src src_srcs)
add_executable (${project_name} ${src_srcs})
target_link_libraries (${project_name} m)

# external packages
add_subdirectory ("${PROJECT_SOURCE_DIR}/pkg/argtable3")
include_directories ("${PROJECT_SOURCE_DIR}/pkg/argtable3")
target_link_libraries (${project_name} argtable3)
