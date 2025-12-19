cmake_minimum_required(VERSION 3.16)
project(vulkan-app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/VulkanApp.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw)

