cmake_minimum_required(VERSION 3.18)
project(learnopengl)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(COPY res DESTINATION ${PROJECT_BINARY_DIR})

add_executable(learnopengl ${SOURCE_FILES})
target_include_directories(learnopengl PUBLIC ${CMAKE_SOURCE_DIR}/include)

set_property(TARGET learnopengl PROPERTY CXX_STANDARD 20)

# find_package(OpenGL REQUIRED)
# find_package(glfw3 REQUIRED)
find_library(OpenGL_LIBRARY OpenGL)
find_package(glfw3 REQUIRED)

target_link_libraries(learnopengl ${OpenGL_LIBRARY} glfw)

