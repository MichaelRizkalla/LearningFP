# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("Cpp-FP")

set(INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/../bin)

# Include sub-projects.
add_subdirectory ("CalculateDiscountsOnOrders")
add_subdirectory ("CompositionHelperTests")
add_subdirectory("CompositionExample")
