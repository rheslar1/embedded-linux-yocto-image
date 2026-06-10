cmake_minimum_required(VERSION 3.16)
project(embedded_linux_yocto_image LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_library(yocto_image_core
  src/YoctoImage.cpp
)
target_include_directories(yocto_image_core PUBLIC include)
target_compile_options(yocto_image_core PRIVATE -Wall -Wextra -Wpedantic)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE yocto_image_core)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

add_executable(yocto_image_tests tests/YoctoImageTests.cpp)
target_link_libraries(yocto_image_tests PRIVATE yocto_image_core)
target_compile_options(yocto_image_tests PRIVATE -Wall -Wextra -Wpedantic)
add_test(NAME yocto_image_tests COMMAND yocto_image_tests)
