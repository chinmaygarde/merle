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

project(neon_sandbox
  LANGUAGES
    C
    CXX
    OBJC
    OBJCXX
)

enable_testing()

add_subdirectory(third_party/sdl)
add_subdirectory(third_party/googletest)
set(GOOGLETEST_PATH "third_party/googletest")
set(BENCHMARK_ENABLE_TESTING FALSE)
add_subdirectory(third_party/googlebenchmark)
add_subdirectory(third_party/glm)

include(GoogleTest)

add_library(neon_sandbox_lib
  src/geom.h
  src/macros.h
  src/texture.cc
  src/texture.h
)

target_compile_options(neon_sandbox_lib
  PRIVATE
    -Werror
    -Wall
    -Wextra
    -Wno-unused-parameter
    -Wno-missing-field-initializers
    -Wno-deprecated-declarations
)

add_executable(neon_sandbox
  src/application.cc
  src/application.h
  src/playground.cc
  src/playground.h
  src/sdl_utils.cc
  src/sdl_utils.h
  src/test_runner.cc
  src/test_runner.h
  src/tests.cc
)

target_link_libraries(neon_sandbox
  PUBLIC
    glm
    gtest_main
    neon_sandbox_lib
    SDL3
)

get_filename_component(NS_ASSETS_LOCATION assets ABSOLUTE)
configure_file(src/fixtures_location.h.in fixtures_location.h @ONLY)
target_include_directories(neon_sandbox PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

gtest_discover_tests(neon_sandbox)

add_executable(neon_sandbox_benchmarks
  src/benchmarks.cc
)

target_link_libraries(neon_sandbox_benchmarks neon_sandbox_lib benchmark::benchmark)
target_include_directories(neon_sandbox_benchmarks PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

