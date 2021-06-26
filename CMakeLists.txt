cmake_minimum_required(VERSION 3.18)
project(learnopengl)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(COPY shader DESTINATION ${PROJECT_BINARY_DIR})

add_executable(learnopengl ${SOURCE_FILES})

target_link_libraries(learnopengl GL glfw)