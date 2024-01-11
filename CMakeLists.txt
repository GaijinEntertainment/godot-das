# This file is for convinient building of Daslang

cmake_minimum_required(VERSION 3.27)
project(godot-das)

# remove for release
set(CMAKE_BUILD_TYPE Debug)

# don't build any binaries
set(DAS_TESTS_DISABLED ON)
set(DAS_PROFILE_DISABLED ON)
set(DAS_TUTORIAL_DISABLED ON)
set(DAS_TOOLS_DISABLED ON)
set(DAS_AOT_EXAMPLES_DISABLED ON)
set(DAS_GLFW_DISABLED ON)

add_subdirectory(daScript)

set_target_properties(libDaScript PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../lib")
