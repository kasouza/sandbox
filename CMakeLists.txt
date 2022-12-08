cmake_minimum_required(VERSION 3.21.3)
project(sandbox)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_C_STANDARD 17)
set(LIBS_DIR libs)

# Libraries
add_subdirectory(${LIBS_DIR}/glfw)
add_subdirectory(${LIBS_DIR}/glad)
add_subdirectory(${LIBS_DIR}/stb_image)

# Executable
add_subdirectory(src)
