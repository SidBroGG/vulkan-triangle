#include "VulkanApp.h"

#include <iostream>
#include <stdexcept>
#include <vector>

void VulkanApp::run() {
    CreateWindow();
    InitVulkan();
    MainLoop();
    Cleanup();
}

void VulkanApp::InitVulkan() {
    CreateInstance();
    PrintExts();
    PickPhysicalDevice();
    CreateLogicalDevice();
}

void VulkanApp::MainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

void VulkanApp::Cleanup() {
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

// Window part
void VulkanApp::CreateWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(800, 600, "Pukan triangle", nullptr, nullptr);
}

// Vulkan part
void VulkanApp::CreateInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Pukan triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    uint32_t glfwExtCount = 0;
    const char** glfwExt = glfwGetRequiredInstanceExtensions(&glfwExtCount);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtCount;
    createInfo.ppEnabledExtensionNames = glfwExt;
    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS)
        throw std::runtime_error("Failed to create instance");
}

void VulkanApp::PickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
        throw std::runtime_error("Failed to find GPU with vulkan support");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (IsDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find suitable GPU");
    }
}

void VulkanApp::CreateLogicalDevice() {
    QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledExtensionNames = nullptr;

    VkResult result =
        vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
}

// Debug part
void VulkanApp::PrintExts() {
    uint32_t extCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
    std::vector<VkExtensionProperties> exts(extCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extCount, exts.data());

    std::cout << "Available exensions:\n";

    for (const auto& ext : exts) {
        std::cout << ext.extensionName << "; ";
    }
    std::cout << std::endl;
}

// Auxiliary funcs
bool VulkanApp::IsDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = FindQueueFamilies(device);

    return indices.graphicsFamily.has_value();
}

QueueFamilyIndices VulkanApp::FindQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                             nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                             queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}
