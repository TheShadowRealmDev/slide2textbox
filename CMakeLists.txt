cmake_minimum_required(VERSION 3.21)
project(UnlimitedSliders VERSION 0.1.0)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Defina a variável de ambiente GEODE_SDK apontando pro geode-sdk (o CLI faz isso sozinho: `geode new`).")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

file(GLOB SOURCES src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries(${PROJECT_NAME} geode-sdk)

setup_geode_mod(${PROJECT_NAME})
