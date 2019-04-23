cmake_minimum_required (VERSION 2.8)

# configs
set (project_name ehp)
set (version_major 1)
set (version_minor 0)
set (version_patch 0)
set (www_root root)
set (log_file ehp.log)
set (default_log_file ehp.log)

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

# source files
aux_source_directory (src src_objs)
add_executable (${project_name} ${src_objs})
target_link_libraries (${project_name} m gc)

# external package - argtable3
add_subdirectory ("${PROJECT_SOURCE_DIR}/pkg/argtable3")
include_directories ("${PROJECT_SOURCE_DIR}/pkg/argtable3")
target_link_libraries (${project_name} argtable3)

# testing
enable_testing()
add_subdirectory ("${PROJECT_SOURCE_DIR}/test")
