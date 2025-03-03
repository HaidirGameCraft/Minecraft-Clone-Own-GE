cmake_minimum_required(VERSION 3.22.1)

project(engine)
set(OPENGL_LIBRARY "/usr/lib/libGL.so.1.7.0")

file(GLOB CXX_SOURCE CMAKE_CONFIGURE_DEPENDS "src/*.cpp" "src/source/*.cpp")
file(GLOB C_SOURCE CMAKE_CONFIGURE_DEPENDS "src/*.c" "src/source/*.c")

add_executable(${PROJECT_NAME} main.cpp ${CXX_SOURCE} ${C_SOURCE})
target_include_directories(${PROJECT_NAME} PUBLIC "src/" "include/" "src/source/")
target_link_directories(${PROJECT_NAME} PRIVATE "library/")
target_link_libraries(${PROJECT_NAME} PUBLIC libglfw3.a "/usr/lib64/libGL.so.1")