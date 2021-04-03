
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CompositionHelperTests "main.cpp" "CompositionHelperTests.h")

target_include_directories(CompositionHelperTests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../CompositionHelper/")
