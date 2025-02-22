cmake_minimum_required(VERSION 3.18)
project(learnopengl)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(COPY resource DESTINATION ${PROJECT_BINARY_DIR})

add_executable(learnopengl ${SOURCE_FILES})
target_include_directories(learnopengl PUBLIC ${CMAKE_SOURCE_DIR}/include)

set_property(TARGET learnopengl PROPERTY CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
if (APPLE)
  set(LIBS ${OPENGL_LIBRARIES} glfw)
elseif(UNIX AND NOT APPLE)
  set(LIBS GL glfw)
endif (APPLE)

target_link_libraries(learnopengl ${LIBS})

