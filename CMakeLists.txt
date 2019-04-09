cmake_minimum_required (VERSION 2.8)

set (project_name ehp)
set (ehp_version_major 0)
set (ehp_version_minor 0)
set (ehp_version_patch 0)

set (ehp_www_root root)

set (ehp_log_file ehp.log)
set (ehp_default_log_file ehp.log)

project (${project_name})
add_compile_options(-std=c99)

# configuration
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
