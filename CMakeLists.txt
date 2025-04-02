cmake_minimum_required(VERSION 3.10)
project(Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(ENGINE_PATH ${CMAKE_SOURCE_DIR}/Engine/libs)

# Force static linking of the standard library (for MinGW)
if (MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()

# Add GLFW as a subdirectory
add_subdirectory(${ENGINE_PATH}/glfw)

# Add GLM as a subdirectory (if it has a CMakeLists.txt)
add_subdirectory(${ENGINE_PATH}/glm)

# Add GLAD manually
add_library(glad STATIC ${ENGINE_PATH}/glad/src/glad.c)
target_include_directories(glad PUBLIC ${ENGINE_PATH}/glad/include)

# Add the executable
file(GLOB CPP "src/*.cpp")
file(GLOB H "src/*.h")
add_executable(Game ${CPP} ${H})

# Include GLFW, GLAD, stb, and GLM headers
target_include_directories(Game PRIVATE 
    ${ENGINE_PATH}/glfw/include 
    ${ENGINE_PATH}/glad/include 
    ${ENGINE_PATH}/stb  
    ${ENGINE_PATH}/glm
)

# Link GLFW, GLAD, and necessary system libraries
target_link_libraries(Game PRIVATE glfw glad opengl32 gdi32 user32)
