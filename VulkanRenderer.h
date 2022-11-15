#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept> // Check the err msg manually 
#include <vector>

#include "Utilities.h"

class VulkanRenderer
{
private:
	GLFWwindow* window;
	// Vulkan Components: 
	VkInstance instance; // Vulkan types started with capital V
	// Devices related structure and structure instance:
	struct 
	{
		VkPhysicalDevice physicalDevice{};
		VkDevice logicalDevice{};
	} devices;
	// Vulkan queue family for the device
	VkQueue graphicsQueue{};


private: 

	// 1) Create the instance:
	void createInstance();
	// 2) Get the physical device 
	void getPhysicalDevice(); 
	// 3) Create logical device
	void createLogicalDevice(); 

	// Helper Functions: 
	bool checkInstanceExtensionSupport(std::vector<const char*>* inputExtensions);
	bool checkInstanceDeviceSupport(VkPhysicalDevice inputPhysicalDevice);
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice inputPhysicalDevice);

public: 
	VulkanRenderer();

	int init(GLFWwindow * newWindow);
	void cleanup(); 

	~VulkanRenderer(); 
};

