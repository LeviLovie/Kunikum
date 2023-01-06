cmake_minimum_required(VERSION 3.0.0)
project(kunikum VERSION 0.1.0)

include(CTest)
enable_testing()

set(CPP
    src/main.cpp
    src/VRAM.cpp
    src/reading.cpp
)
set(H
    src/VRAM.h
    src/reading.h
)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
add_executable(kunikum ${CPP} ${H})
target_link_libraries(kunikum sfml-graphics)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
