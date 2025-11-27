cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CCACHE_PROGRAM NAMES ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools")

project(merle
  LANGUAGES
    C
    CXX
    ISPC
)

if(APPLE)
  enable_language(OBJC)
  enable_language(OBJCXX)
endif()

option(MERLE_ENABLE_TESTS "Build the playgrounds and unittests." ON)
option(MERLE_ENABLE_BENCHMARKS "Build benchmarks." ON)

enable_testing()
include(GoogleTest)

find_program(ISPC_PROGRAM ispc)

add_custom_command(
  OUTPUT texture_ispc.o gen/texture_ispc.h
  COMMAND ${ISPC_PROGRAM} --werror
                          -O3
                          -o texture_ispc.o
                          -h gen/texture_ispc.h
                          ${CMAKE_SOURCE_DIR}/src/texture.ispc
  DEPENDS src/texture.ispc
)

add_library(merle
  src/geom.h
  src/ispc_tasksys.cc
  src/macros.h
  src/texture.cc
  src/texture.h
  ${CMAKE_BINARY_DIR}/texture_ispc.o
)

target_compile_options(merle
  PRIVATE
    -Werror
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -Wno-deprecated-declarations
)

find_package(Stb REQUIRED)
target_include_directories(merle
  PUBLIC
    ${Stb_INCLUDE_DIR}
    ${CMAKE_BINARY_DIR}/gen
)

get_filename_component(NS_ASSETS_LOCATION assets ABSOLUTE)
configure_file(src/fixtures_location.h.in fixtures_location.h @ONLY)

if(MERLE_ENABLE_TESTS)
  find_package(SDL3 CONFIG REQUIRED)
  find_package(GTest CONFIG REQUIRED)
  find_package(imgui CONFIG REQUIRED)

  add_executable(merle_tests
    src/application.cc
    src/application.h
    src/sdl_utils.cc
    src/sdl_utils.h
    src/test_runner.cc
    src/test_runner.h
    src/test_runner.h
    src/tests.cc
  )

  target_link_libraries(merle_tests
    PUBLIC
      GTest::gtest
      GTest::gtest_main
      merle
      SDL3::SDL3
      imgui::imgui
  )

  target_include_directories(merle_tests
    PUBLIC
      ${CMAKE_CURRENT_BINARY_DIR}
  )

  # gtest_discover_tests(merle_tests)
endif(MERLE_ENABLE_TESTS)

if(MERLE_ENABLE_BENCHMARKS)
  find_package(benchmark CONFIG REQUIRED)
  add_executable(merle_benchmarks
    src/benchmarks.cc
  )
  target_link_libraries(merle_benchmarks merle benchmark::benchmark)
  target_include_directories(merle_benchmarks
    PUBLIC
      ${CMAKE_CURRENT_BINARY_DIR}
  )
endif()

