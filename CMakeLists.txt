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

add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL)
add_subdirectory(third_party/sdl EXCLUDE_FROM_ALL)
set(GOOGLETEST_PATH "third_party/googletest" EXCLUDE_FROM_ALL)
set(BENCHMARK_ENABLE_TESTING FALSE)
add_subdirectory(third_party/googlebenchmark EXCLUDE_FROM_ALL)

enable_testing()
include(GoogleTest)

add_library(merle
  src/geom.h
  src/macros.h
  src/texture.cc
  src/texture.h
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

target_include_directories(merle
  PUBLIC
    third_party/stb
)

add_library(merle_ispc
  src/texture.ispc
)

if (APPLE)
set_property(TARGET merle_ispc PROPERTY ISPC_INSTRUCTION_SETS neon-i32x8)
else()
set_property(TARGET merle_ispc PROPERTY ISPC_INSTRUCTION_SETS avx2-i32x16 avx512skx-x64)
endif()

target_compile_options(merle_ispc PRIVATE "--werror")

target_link_libraries(merle
  PUBLIC
    merle_ispc
)

get_filename_component(NS_ASSETS_LOCATION assets ABSOLUTE)
configure_file(src/fixtures_location.h.in fixtures_location.h @ONLY)

if(MERLE_ENABLE_TESTS)
  add_executable(merle_tests
    src/application.cc
    src/application.h
    src/sdl_utils.cc
    src/sdl_utils.h
    src/test_runner.cc
    src/test_runner.h
    src/test_runner.h
    src/tests.cc
    third_party/imgui/backends/imgui_impl_sdl3.cpp
    third_party/imgui/backends/imgui_impl_sdl3.h
    third_party/imgui/backends/imgui_impl_sdlrenderer3.cpp
    third_party/imgui/backends/imgui_impl_sdlrenderer3.h
    third_party/imgui/imconfig.h
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui.h
    third_party/imgui/imgui_demo.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_internal.h
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp
  )

  target_link_libraries(merle_tests
    PUBLIC
      gtest_main
      merle
      SDL3::SDL3
  )

  target_include_directories(merle_tests
    PUBLIC
      ${CMAKE_CURRENT_BINARY_DIR}
      third_party/imgui
  )

  gtest_discover_tests(merle_tests)
endif(MERLE_ENABLE_TESTS)

if(MERLE_ENABLE_BENCHMARKS)
  add_executable(merle_benchmarks
    src/benchmarks.cc
  )
  target_link_libraries(merle_benchmarks merle benchmark::benchmark)
  target_include_directories(merle_benchmarks PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
endif()

