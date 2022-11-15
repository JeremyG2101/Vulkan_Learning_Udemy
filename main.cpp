// 2022-11-14
// Created by Jeremy Gong

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept> // Check the err msg manually 
#include <vector>

#include "VulkanRenderer.h"

GLFWwindow* window; 
VulkanRenderer vulkanRenderer;

void initWindow(std::string windowName = "Test Window", const int width = 800, const int height = 600) 
{
	// Initialize GLFW 
	glfwInit(); 

	// Set GLFW to NOT work with OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); 
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); 

	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr); 
}

int main()
{
	// Create Window 
	initWindow(); // use default values

	// Create Vulkan Renderer Instance
	if (vulkanRenderer.init(window) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	// Loop until closed
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	vulkanRenderer.cleanup();

	glfwDestroyWindow(window);
	glfwTerminate(); 

	return 0;
}