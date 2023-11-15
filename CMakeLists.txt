cmake_minimum_required(VERSION 3.27)
project(Godas)

link_directories(daScript)
include_directories(daScript/include gdextension)

# don't build any binaries
set(DAS_TESTS_DISABLED ON)
set(DAS_PROFILE_DISABLED ON)
set(DAS_TUTORIAL_DISABLED ON)
set(DAS_TOOLS_DISABLED ON)
set(DAS_AOT_EXAMPLES_DISABLED ON)
set(DAS_GLFW_DISABLED ON)

add_subdirectory(daScript)
# required as we build shared library as target
set_target_properties(libDaScript PROPERTIES POSITION_INDEPENDENT_CODE ON)


add_library(godotdas SHARED src/entry_point.cpp src/tutorial00.cpp)
target_link_libraries(godotdas libDaScript)
add_dependencies(godotdas libDaScript)
set_target_properties(godotdas PROPERTIES LIBRARY_OUTPUT_DIRECTORY demo/bin)
