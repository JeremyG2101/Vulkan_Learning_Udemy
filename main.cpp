//#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN // this lets GLFW to include Vulkan for us
#include <GLFW/glfw3.h> // Include GLFW

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

int main()
{
	// Check if GLFW is working...
	glfwInit(); // This basically tests if GLFW initializes itself. 
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // we want to make sure that it sets for no API
	
	// Create a window
	GLFWwindow * window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr); 

	// Check if Vulkan is working...
	uint32_t extensionCount = 0; // this is just an unsigned int, from Vulkan format
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr); // vk for function and TYPE for Vk

	printf("Extension count: %i\n", extensionCount); // Check how many extensions it supports. Always update graphic driver to support more extensions.

	// Check if GLM is working...
	glm::mat4 testMatrix(1.0f); 
	glm::vec4 testVector(1.0f); 

	auto testResults = testMatrix * testVector; 

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // this just polls events, and in our case, its a clean set
	}

	glfwDestroyWindow(window); 
	glfwTerminate(); // Deactivate GLFW

	return 0;
}