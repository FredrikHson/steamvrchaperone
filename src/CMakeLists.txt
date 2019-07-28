cmake_minimum_required(VERSION 3.4)

project(chaperone)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(COMMON_SRC
    "main.c"
    "vr_bullshit.cpp"
    )

include_directories(
    "."
    )

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_executable(${CMAKE_PROJECT_NAME} ${COMMON_SRC})

target_link_libraries(${CMAKE_PROJECT_NAME} openvr_api)
