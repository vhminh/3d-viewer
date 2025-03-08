cmake_minimum_required(VERSION 3.18)
project(3d-viewer)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(COPY resource DESTINATION ${PROJECT_BINARY_DIR})

add_executable(3d-viewer ${SOURCE_FILES})
target_include_directories(3d-viewer PUBLIC ${CMAKE_SOURCE_DIR}/include)

set_property(TARGET 3d-viewer PROPERTY CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include (FetchContent)
FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG        v5.4.3
)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=array-bounds")
FetchContent_MakeAvailable(assimp)
set_property(TARGET assimp PROPERTY COMPILE_FLAGS "-Wno-error=array-bounds")
target_include_directories(3d-viewer PRIVATE ${FETCHCONTENT_BASE_DIR}/assimp-src/include)

if (APPLE)
  set(LIBS ${OPENGL_LIBRARIES} glfw assimp)
elseif(UNIX AND NOT APPLE)
  set(LIBS GL glfw assimp)
endif (APPLE)

target_link_libraries(3d-viewer ${LIBS})

# misc
add_executable(down_scale_textures ${CMAKE_SOURCE_DIR}/misc/down_scale_textures.cpp)
set_property(TARGET down_scale_textures PROPERTY CXX_STANDARD 20)
target_include_directories(down_scale_textures PUBLIC ${CMAKE_SOURCE_DIR}/include)

