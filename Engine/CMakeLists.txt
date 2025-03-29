cmake_minimum_required(VERSION 3.10)
project(Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add GLFW as a subdirectory
add_subdirectory(libs/glfw)

# Add GLAD manually
add_library(glad STATIC libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)

# Add the executable
file(GLOB CPP "src/*.cpp")
file(GLOB H "src/*.h")
add_executable(Engine ${CPP} ${H})

# Include GLFW and GLAD headers
target_include_directories(Engine PRIVATE 
    libs/glfw/include 
    libs/glad/include 
    libs/stb  
)

# Link GLFW, GLAD, and necessary system libraries
target_link_libraries(Engine PRIVATE glfw glad opengl32.lib gdi32.lib user32.lib)
