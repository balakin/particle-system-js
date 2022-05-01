cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(ParticleSystem)

if(NOT EMSCRIPTEN)
  message(FATAL_ERROR "Emscripten not found")
endif(NOT EMSCRIPTEN)

file(
  GLOB_RECURSE SRC_FILES
  "${PROJECT_SOURCE_DIR}/src/particle-system/**/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/particle-system/**/*.hpp"
)
add_executable(ParticleSystem ${SRC_FILES})
include_directories("src/particle-system/vendor" "src/particle-system/vendor/imgui")

set_property(TARGET ParticleSystem PROPERTY CXX_STANDARD 17)

target_compile_options(ParticleSystem PRIVATE
  -sUSE_SDL=2
)

target_link_options(ParticleSystem PRIVATE
  -sEXPORT_ES6
  -sMODULARIZE
  -sENVIRONMENT=web
  -sMAX_WEBGL_VERSION=2
  -sUSE_SDL=2
  -sALLOW_MEMORY_GROWTH
  "SHELL:--output_eol linux"
  "SHELL:--embed-file \"${PROJECT_SOURCE_DIR}/src/particle-system/assets@assets\""
)

# Debug
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Release
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
