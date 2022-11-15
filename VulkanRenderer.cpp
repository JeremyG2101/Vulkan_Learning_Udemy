#include "VulkanRenderer.h"

void VulkanRenderer::createInstance()
{

	// 1) Vulkan Application Information: 
	VkApplicationInfo appInfo{}; // initialize it

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;  
	appInfo.pApplicationName = "Vulkan Application"; 
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); 
	appInfo.pEngineName = "No Engine"; 
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); 
	// The following is to manually add application version
	appInfo.apiVersion = VK_API_VERSION_1_0;  // this is to set the Vulkan API version

	// 2) Create a list to hold instance extensions and let GLFW detect them automatically 
	std::vector<const char*> instanceExtensions{};

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	// Populate the extensions and get the extension count by passing reference of the instanceCount
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); // automatically detect extensions

	// Add extensions to the extension vector so it can be passed to the p-value 
	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}

	// Adding a helper function checking the extension support
	if (!checkInstanceExtensionSupport(&instanceExtensions))
	{
		throw std::runtime_error("VkInstance does not support required extensions!");
	}

	// 3) Main: Vulkan Instance Create Information:
	VkInstanceCreateInfo instanceCreateInfo{}; // initialize it

	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.pApplicationInfo = &appInfo; // for a p variable has to pass by reference 
	// Adding and enabling extensions
	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

	// 4) Finally to create Instance 
	// It is created using the standard Vulkan Creation format:
	VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance); 

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a Vulkan Instance!");
	}
}

void VulkanRenderer::getPhysicalDevice()
{
	// Enumerate he physical device list 
	uint32_t deviceCount = 0; 
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr); 

	if (deviceCount == 0)
	{
		throw std::runtime_error("Can't find GPUs that supports Vulkan Instance!");
	}

	// Get a list of Physical Devices using the standard Vulkan format
	std::vector<VkPhysicalDevice> deviceList(deviceCount); 
	vkEnumeratePhysicalDevices(instance, &deviceCount, deviceList.data()); // enumrate begin with the first pointer of the device list
	
	bool isQueue = false;
	for (const auto& device : deviceList)
	{
		if (checkInstanceDeviceSupport(device))
		{
			devices.physicalDevice = device;
			isQueue = true;
			break;
		}
	}

	if (!isQueue)
	{
		throw std::runtime_error("Can't find suitable graphic queue families!");
	}

}

void VulkanRenderer::createLogicalDevice()
{
	//Sync the Graphic Family 
	QueueFamilyIndices indices = getQueueFamilies(devices.physicalDevice); 

	// 1) Create queue information: 
	float priority = 1.0f;

	VkDeviceQueueCreateInfo queueCreateInfo{}; // always initialize it 

	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicFamily;
	queueCreateInfo.queueCount = 1; // since only one queue is used in this instance 
	queueCreateInfo.pQueuePriorities = &priority; // 1 is the highest priority


	// 2) Create device feature(s):
	VkPhysicalDeviceFeatures deviceFeatures{}; // leave it empty for this instance by initializing it

	// 3) Create device Information: 
	VkDeviceCreateInfo deviceCreateInfo{}; // initialize it
	
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = 1; // since only 1 logical device will be used in this insitance
	deviceCreateInfo.enabledExtensionCount = 0; // no logical device extensions will be enables
	deviceCreateInfo.ppEnabledExtensionNames = nullptr; 

	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo; 
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;


	// 4) Create the logical device, using standard Vulkan format
	// first to sync the logical device with its physical device, and then sync te logical device create information to it
	VkResult result = vkCreateDevice(devices.physicalDevice, &deviceCreateInfo, nullptr, &devices.logicalDevice); 
	
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a logical device!");
	}

	// 5) Create physical device/graphic queue:
	vkGetDeviceQueue(devices.logicalDevice, indices.graphicFamily, 0, &graphicsQueue); 
	// the queue index can be also replaced by the following line: 
	//vkGetDeviceQueue(devices.logicalDevice, queueCreateInfo.queueFamilyIndex, 0, &graphicsQueue);
}

bool VulkanRenderer::checkInstanceExtensionSupport(std::vector<const char*>* inputExtensions)
{
	
	// Get the numbers of extensions
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr); // this is to get the total extensions

	// Create a list of VkExtensionProperties 
	std::vector<VkExtensionProperties> extensions(extensionCount); // this is to pre-allocate the vector
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());  // sync to the extension vector

	// Check if extensions are in the list of input extensions 
	for (const auto& inputExtension : *inputExtensions)
	{
		bool hasExtension = false; 
		for (const auto& extension : extensions)
		{
			if (strcmp(inputExtension, extension.extensionName))
			{
				hasExtension = true;
				break;
			}
		}
		if (!hasExtension)
		{
			return false;
		}
	}
	
	return true;
}

bool VulkanRenderer::checkInstanceDeviceSupport(VkPhysicalDevice inputPhysicalDevice)
{
	QueueFamilyIndices indices = getQueueFamilies(inputPhysicalDevice); 
	return indices.isValid();
}

QueueFamilyIndices VulkanRenderer::getQueueFamilies(VkPhysicalDevice inputPhysicalDevice)
{
	QueueFamilyIndices indices{};

	// Get the queue families using standard Vuljan format
	uint32_t queueFamilyCount = 0; 
	vkGetPhysicalDeviceQueueFamilyProperties(inputPhysicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount); // pre-allocate the vector before sync
	vkGetPhysicalDeviceQueueFamilyProperties(inputPhysicalDevice, &queueFamilyCount, queueFamilies.data()); // sync it and let it read though the first vector.
	
	// Go through each and every queue family until the correct one was found, then pass its index 
	int i = 0; 
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
		{
			indices.graphicFamily = i;
		}
		// if there already is one suit for the physical device, then break and mark the indice
		if (indices.isValid())
		{
			break;
		}

		i++;
	}
	
	return indices;
}

VulkanRenderer::VulkanRenderer() {}

int VulkanRenderer::init(GLFWwindow* newWindow)
{
	window = newWindow;

	// Using a try throw and catch to get the debug information
	try 
	{
		createInstance();
		getPhysicalDevice(); 
		createLogicalDevice();
	}
	catch (const std::runtime_error& e) 
	{ 
		printf("ERROR: %s\n", e.what()); 
		return EXIT_FAILURE; 
	}

	return 0;
}

void VulkanRenderer::cleanup()
{
	vkDestroyDevice(devices.logicalDevice, nullptr);
	vkDestroyInstance(instance, nullptr);
}

VulkanRenderer::~VulkanRenderer() {}
