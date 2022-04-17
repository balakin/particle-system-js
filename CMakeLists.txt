cmake_minimum_required(VERSION 3.2)

project(ParticleSystem)

file(GLOB SRC_FILES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/particle-system/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/particle-system/src/*.hpp")
message(STATUS ${SRC_FILES})
add_executable(ParticleSystem ${SRC_FILES})
include_directories("src/particle-system/vendor")

if(NOT EMSCRIPTEN)
  message(FATAL_ERROR "Emscripten not found")
endif(NOT EMSCRIPTEN)

target_link_options(ParticleSystem PRIVATE
  -sEXPORT_ES6
  -sMODULARIZE
  -sENVIRONMENT=web
  -sMAX_WEBGL_VERSION=2
  -lembind
  "SHELL:--output_eol linux"
)

# Debug
set(CMAKE_CXX_FLAGS_DEBUG "-g")

# Release
set(CMAKE_CXX_FLAGS_RELEASE "-Os -DNDEBUG")