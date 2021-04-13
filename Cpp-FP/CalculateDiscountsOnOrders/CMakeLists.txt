# CMakeList.txt : CMake project for CalculateDiscountsOnOrders, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.19)

# Add source to this project's executable.
add_executable (CalculateDiscountsOnOrders "main.cpp" "CalculateDiscountsOnOrders.h")

target_include_directories(CalculateDiscountsOnOrders PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../FPHelper/")

install(TARGETS CalculateDiscountsOnOrders RUNTIME DESTINATION ${INSTALL_DIR}/)
