#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VulkanApp {
   public:
    void run();

   private:
    GLFWwindow* window;

    VkInstance instance;
    VkPhysicalDevice physicalDevice;

    void InitVulkan();
    void MainLoop();
    void Cleanup();

    // Window
    void CreateWindow();

    // Vulkan part
    void CreateInstance();
    void PickPhysicalDevice();

    // Debug
    void PrintExts();

    // Auxiliary funcs
    bool IsDeviceSuitable(VkPhysicalDevice device);
};