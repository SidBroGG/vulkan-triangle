#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() { return graphicsFamily.has_value(); }
};

class VulkanApp {
   public:
    void run();

   private:
    GLFWwindow* window;

    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue graphicsQueue;

    void InitVulkan();
    void MainLoop();
    void Cleanup();

    // Window
    void CreateWindow();

    // Vulkan part
    void CreateInstance();
    void PickPhysicalDevice();
    void CreateLogicalDevice();

    // Debug
    void PrintExts();

    // Auxiliary funcs
    bool IsDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
};