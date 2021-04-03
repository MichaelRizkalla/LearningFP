
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CompositionExample "main.cpp" "CompositionExample.h")

target_include_directories(CompositionExample PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../CompositionHelper/")
