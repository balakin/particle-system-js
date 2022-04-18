cmake_minimum_required(VERSION 3.2)

project(ParticleSystem)

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/particle-system/src/" *.cpp *.hpp)
add_executable(ParticleSystem ${SRC_FILES})
include_directories("src/particle-system/vendor" "src/particle-system/vendor/imgui")

if(NOT EMSCRIPTEN)
  message(FATAL_ERROR "Emscripten not found")
endif(NOT EMSCRIPTEN)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/src/particle-system/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

set_property(TARGET ParticleSystem PROPERTY CXX_STANDARD 17)

target_link_options(ParticleSystem PRIVATE
  -sEXPORT_ES6
  -sMODULARIZE
  -sENVIRONMENT=web
  -sMAX_WEBGL_VERSION=2
  -sUSE_GLFW=3
  -sALLOW_MEMORY_GROWTH
  "SHELL:--output_eol linux"
  "SHELL:--embed-file assets"
)

# Debug
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Release
set(CMAKE_CXX_FLAGS_RELEASE "-Os -DNDEBUG")