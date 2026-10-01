cmake_minimum_required(VERSION 4.0)

find_package(cmake-bare REQUIRED PATHS node_modules/cmake-bare)

project(bare_core_text C OBJC)

add_bare_module(bare_core_text)

target_sources(
  ${bare_core_text}
  PRIVATE
    binding.m
)

target_include_directories(
  ${bare_core_text}
  PRIVATE
    node_modules/bare-foundation-registry/lib
)

target_link_libraries(
  ${bare_core_text}
  PUBLIC
    "-framework CoreText"
    "-framework CoreGraphics"
)
