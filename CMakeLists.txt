cmake_minimum_required(VERSION 3.10)
project(libcyaml VERSION 2.0.0 LANGUAGES C)

# Version defines
add_compile_definitions(
    VERSION_MAJOR=2
    VERSION_MINOR=0
    VERSION_PATCH=0
    VERSION_DEVEL=1
)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Source files
set(LIBCYAML_SRC
    src/mem.c
    src/free.c
    src/load.c
    src/save.c
    src/copy.c
    src/util.c
    src/utf8.c
)

# Include directories
include_directories(include)

# Find libyaml
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBYAML REQUIRED yaml-0.1)
include_directories(${LIBYAML_INCLUDE_DIRS})
link_directories(${LIBYAML_LIBRARY_DIRS})

# Build shared library
add_library(cyaml_shared SHARED ${LIBCYAML_SRC})
set_target_properties(cyaml_shared PROPERTIES OUTPUT_NAME cyaml)
target_link_libraries(cyaml_shared ${LIBYAML_LIBRARIES})

# Build static library
add_library(cyaml_static STATIC ${LIBCYAML_SRC})
set_target_properties(cyaml_static PROPERTIES OUTPUT_NAME cyaml)
target_link_libraries(cyaml_static ${LIBYAML_LIBRARIES})

# Examples
add_executable(planner examples/planner/main.c)
target_link_libraries(planner cyaml_static ${LIBYAML_LIBRARIES})

add_executable(numerical examples/numerical/main.c)
target_link_libraries(numerical cyaml_static ${LIBYAML_LIBRARIES})