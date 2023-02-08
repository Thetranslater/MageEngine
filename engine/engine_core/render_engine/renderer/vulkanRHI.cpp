#include"engine_core/render_engine/renderer/vulkanRHI.h"
#include"engine_core/render_engine/renderer/vulkanHelper.h"
#include"engine_core/render_engine/renderer/vulkanInfo.h"

#include"core/macro.h"

#include<set>
#include<algorithm>

namespace Mage {

#ifndef NDEBUG
	bool VulkanRHI::m_enable_validation_layers{ true };
	bool VulkanRHI::m_enable_debug_utils_label{ true };
#else
	bool VulkanRHI::m_enable_validation_layers{ false };
	bool VulkanRHI::m_enable_debug_utils_label{ false };
#endif // !NDEBUG


	std::vector<const char*> VulkanRHI::m_validation_layers{ "VK_LAYER_KHRONOS_validation" };

	std::vector<const char*> VulkanRHI::m_device_extensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME }
	;
	VulkanRHI::~VulkanRHI() {
		
	}

	void VulkanRHI::initialize(std::shared_ptr<WindowSystem> window_system) {
		m_window = window_system->getGLFWWindow();

		createInstance();

		setupDebugMessenger();

		createSurface();

		createPhysicalDevice();

		createDevice();

		createSwapchain();

		createSwapchainImageViews();

		createDepthImage();

		createCommandPools();

		createCommandBuffers();

		createDescriptorPool();

		createSynchronizationPrimitives();
	}

	VkCommandBuffer VulkanRHI::recordCommandsTemporarily() {
		VkCommandBufferAllocateInfo alloc_command_buffer = VulkanInfo::aboutVkCommandBufferAllocateInfo();
		alloc_command_buffer.commandPool = m_command_pools[1];
		alloc_command_buffer.commandBufferCount = 1;
		alloc_command_buffer.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		VkCommandBuffer command_buffer;
		vkAllocateCommandBuffers(m_device, &alloc_command_buffer, &command_buffer);

		VkCommandBufferBeginInfo begin_info = VulkanInfo::aboutVkCommandBufferBeginInfo();
		begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(command_buffer, &begin_info);

		return command_buffer;
	}

	void VulkanRHI::submitCommandsTemporarily(VkCommandBuffer command_buffer) {
		vkEndCommandBuffer(command_buffer);

		VkSubmitInfo submit = VulkanInfo::aboutVkSubmitInfo();
		submit.commandBufferCount = 1;
		submit.pCommandBuffers = &command_buffer;

		vkQueueSubmit(m_graphics_queue, 1, &submit, VK_NULL_HANDLE);
		vkQueueWaitIdle(m_graphics_queue);

		vkFreeCommandBuffers(m_device, m_command_pools[1], 1, &command_buffer);
	}

	void VulkanRHI::waitForFences() {
		if (VK_SUCCESS != vkWaitForFences(m_device, 1, &m_fences[m_current_frame_index], VK_TRUE, UINT64_MAX)) {
			MAGE_THROW(failed to wait for fences)
		}
	}

	bool VulkanRHI::prepareVulkanRHIBeforeRender(std::function<void()> pass_recreate) {
		VkResult acquire_result = vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, m_semaphores[m_current_frame_index], VK_NULL_HANDLE, &m_swapchain_image_index);
		//TODO acquire_result for recreate swapchain

		if (acquire_result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapchain(pass_recreate);
			return true;//TODO:是否会造成死锁
		}


		vkResetCommandBuffer(m_command_buffer,0);

		vkResetFences(m_device, 1, &m_fences[m_current_frame_index]);

		VkCommandBufferBeginInfo begin_info = VulkanInfo::aboutVkCommandBufferBeginInfo();
		
		if (VK_SUCCESS != vkBeginCommandBuffer(m_command_buffer, &begin_info)) {
			MAGE_THROW(failed to begin to record command)
		}

		return false;
	}

	void VulkanRHI::prepareVulkanRHIAfterRender() {
		vkEndCommandBuffer(m_command_buffer);

		VkSubmitInfo submit_info = VulkanInfo::aboutVkSubmitInfo();

		submit_info.waitSemaphoreCount = 1;
		VkSemaphore wait_semaphores = m_semaphores[m_current_frame_index];
		submit_info.pWaitSemaphores = &wait_semaphores;
		VkPipelineStageFlags wait_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		submit_info.pWaitDstStageMask = &wait_flags;

		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &m_command_buffer;

		submit_info.signalSemaphoreCount = 1;
		VkSemaphore signal_semaphores = m_present_semaphores[m_current_frame_index];
		submit_info.pSignalSemaphores = &signal_semaphores;

		if (VK_SUCCESS != vkQueueSubmit(m_graphics_queue, 1, &submit_info, m_fences[m_current_frame_index])) {
			MAGE_THROW(failed to submit commands)
		}

		VkPresentInfoKHR present_info = VulkanInfo::aboutVkPresentInfoKHR();

		present_info.waitSemaphoreCount = 1;
		VkSemaphore wait_semaphores_for_present = m_present_semaphores[m_current_frame_index];
		present_info.pWaitSemaphores = &wait_semaphores_for_present;
		present_info.swapchainCount = 1;
		present_info.pSwapchains = &m_swapchain;
		present_info.pImageIndices = &m_swapchain_image_index;

		VkResult present_result = vkQueuePresentKHR(m_present_queue, &present_info);

		//TODO present result to recreate swapchain

		m_current_frame_index = (m_current_frame_index + 1) % m_command_buffers.size();

	}

	//steps
	void VulkanRHI::createInstance() {
		if (m_enable_validation_layers && !VulkanHelper::checkValidationLayerSupport()) {
			MAGE_THROW(not find validation layers!)
		}

		VkApplicationInfo appInfo = VulkanInfo::aboutVkApplicationInfo();
		appInfo.pEngineName = "Mage";
		appInfo.pApplicationName = "mage";
		appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
		appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
		appInfo.apiVersion = VK_API_VERSION_1_2;

		VkInstanceCreateInfo instanceInfo = VulkanInfo::aboutVkInstanceCreateInfo();
		instanceInfo.pApplicationInfo = &appInfo;
		
		auto extensions = VulkanHelper::getRequiredExtensions();
		instanceInfo.ppEnabledExtensionNames = extensions.data();
		instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());

		VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
		if (m_enable_validation_layers) {
			instanceInfo.enabledLayerCount = static_cast<uint32_t>(m_validation_layers.size());
			instanceInfo.ppEnabledLayerNames = m_validation_layers.data();

			VulkanHelper::populateDebugMessengerCreateInfo(debugInfo);
			instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) & debugInfo;
		}
		else {
			instanceInfo.enabledLayerCount = 0;
			instanceInfo.ppEnabledLayerNames = nullptr;
		}

		if (vkCreateInstance(&instanceInfo, nullptr, &m_instance) != VK_SUCCESS) {
			MAGE_THROW(failed to create instance)
		}
	}

	void VulkanRHI::setupDebugMessenger() {
		if (m_enable_validation_layers) {
			VkDebugUtilsMessengerCreateInfoEXT debugInfo{};
			VulkanHelper::populateDebugMessengerCreateInfo(debugInfo);

			if (createDebugUtilsMessengerEXT(m_instance, &debugInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
				MAGE_THROW(failed to create debug messenger)
			}
		}
		if (m_enable_debug_utils_label) {
			//TODO
		}
	}

	void VulkanRHI::createSurface() {
		if (VK_SUCCESS != glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface)) {
			MAGE_THROW(failed to create surface)
		}
	}

	void VulkanRHI::createPhysicalDevice() {
		uint32_t deviceCount{ 0 };
		vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

		if (deviceCount != 0) {
			std::vector<VkPhysicalDevice> physical_devices(deviceCount);
			vkEnumeratePhysicalDevices(m_instance, &deviceCount, physical_devices.data());

			//ranking
			std::vector<std::pair<int, VkPhysicalDevice>> rank_of_physical_deivce;
			for (auto& physical_device : physical_devices) {
				int score{ 0 };

				VkPhysicalDeviceProperties properties;
				vkGetPhysicalDeviceProperties(physical_device, &properties);
				if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
					score += 1000;
				}
				else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
					score += 100;
				}

				rank_of_physical_deivce.push_back(std::make_pair(score, physical_device));
			}

			std::sort(rank_of_physical_deivce.begin(), rank_of_physical_deivce.end(),
				[](const std::pair<int, VkPhysicalDevice>& l, const std::pair<int, VkPhysicalDevice>& r)
				{
					return l.first > r.first;
				});

			//find suitable device
			//set required features
			m_physical_device_required_features.samplerAnisotropy = VK_TRUE;

			for (auto& device : rank_of_physical_deivce) {
				if (VulkanHelper::isDeviceSuitable(device.second, m_physical_device_required_features, m_surface)) {
					m_physical_device = device.second;
					//require physical device properties and features
					vkGetPhysicalDeviceProperties(m_physical_device, &m_physical_device_properties);
					vkGetPhysicalDeviceFeatures(m_physical_device, &m_physical_device_supported_features);
					vkGetPhysicalDeviceMemoryProperties(m_physical_device, &m_physical_device_memory_properties);
					break;
				}
			}

			if (m_physical_device == VK_NULL_HANDLE) {
				MAGE_THROW(failed to find a suitable physical device)
			}
		}
		else {
			MAGE_THROW(cant find a physical device)
		}
	}

	void VulkanRHI::createDevice() {
		m_queue_indices = VulkanHelper::findQueueFamily(m_physical_device, m_surface);

		std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
		std::set<uint32_t> queue_families = { m_queue_indices.m_graphics_family.value(),m_queue_indices.m_present_family.value() };

		float queue_priority = 1.f;

		for (auto queue_family : queue_families) {
			VkDeviceQueueCreateInfo queueInfo = VulkanInfo::aboutVkDeviceQueueCreateInfo();
			queueInfo.queueCount = 1;
			queueInfo.queueFamilyIndex = queue_family;
			queueInfo.pQueuePriorities = &queue_priority;
			queue_create_infos.push_back(queueInfo);
		}

		VkDeviceCreateInfo deviceInfo = VulkanInfo::aboutVkDeviceCreateInfo();

		deviceInfo.enabledExtensionCount = static_cast<uint32_t>(m_device_extensions.size());
		deviceInfo.ppEnabledExtensionNames = m_device_extensions.data();
		deviceInfo.enabledLayerCount = 0;
		deviceInfo.ppEnabledLayerNames = nullptr;

		deviceInfo.pEnabledFeatures = &m_physical_device_required_features;

		deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
		deviceInfo.pQueueCreateInfos = queue_create_infos.data();

		//extenson features;
		VkPhysicalDeviceRobustness2FeaturesEXT features{};
		features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT;
		features.nullDescriptor = VK_TRUE;

		deviceInfo.pNext = &features;

		if (vkCreateDevice(m_physical_device, &deviceInfo, nullptr, &m_device) != VK_SUCCESS) {
			MAGE_THROW(failed to create device);
		}

		//get device queues
		vkGetDeviceQueue(m_device, m_queue_indices.m_graphics_family.value(), 0, &m_graphics_queue);
		vkGetDeviceQueue(m_device, m_queue_indices.m_present_family.value(), 0, &m_present_queue);

		//TODO

		//effcient function pointers

		//depth image format	
	}

	void VulkanRHI::createSwapchain(VkSwapchainKHR oldswapchain) {
		SwapchainSupportDetails swapchainDetails	= VulkanHelper::querySwapchainDetails(m_physical_device, m_surface);
		//choose suitable info
		VkSurfaceFormatKHR swapchainFormat			= VulkanHelper::chooseSwapchainSurfaceFormatFromDetails(swapchainDetails.formats);
		VkPresentModeKHR swapchainPresentMode		= VulkanHelper::chooseSwapchainSurfacePresentModeFromDetails(swapchainDetails.present_modes);
		VkExtent2D swapchainExtent					= VulkanHelper::chooseSwapchainSurfaceExtentFromDetails(m_window, swapchainDetails.capabilities);

		uint32_t swapchainMinImageCount{ swapchainDetails.capabilities.minImageCount + 1 };
		if (swapchainDetails.capabilities.maxImageCount > 0 &&
			swapchainDetails.capabilities.minImageCount == swapchainDetails.capabilities.maxImageCount) {
			swapchainMinImageCount -= 1;
		}

		VkSwapchainCreateInfoKHR swapchainInfo = VulkanInfo::aboutVkSwapchainCreateInfoKHR();

		swapchainInfo.surface			= m_surface;
		swapchainInfo.minImageCount		= swapchainMinImageCount;
		swapchainInfo.imageFormat		= swapchainFormat.format;
		swapchainInfo.imageColorSpace	= swapchainFormat.colorSpace;
		swapchainInfo.imageExtent		= swapchainExtent;
		swapchainInfo.imageUsage		= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainInfo.preTransform		= swapchainDetails.capabilities.currentTransform;
		swapchainInfo.presentMode		= swapchainPresentMode;
		swapchainInfo.compositeAlpha	= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.oldSwapchain		= oldswapchain;

		std::vector<uint32_t> indices	= { m_queue_indices.m_graphics_family.value(),m_queue_indices.m_present_family.value() };
		if (m_queue_indices.m_graphics_family.value() != m_queue_indices.m_present_family.value()) {
			swapchainInfo.imageSharingMode		= VK_SHARING_MODE_CONCURRENT;
			swapchainInfo.queueFamilyIndexCount = static_cast<uint32_t>(indices.size());
			swapchainInfo.pQueueFamilyIndices	= indices.data();
		}
		else {
			swapchainInfo.imageSharingMode		= VK_SHARING_MODE_EXCLUSIVE;
		}

		if (VK_SUCCESS != vkCreateSwapchainKHR(m_device, &swapchainInfo, nullptr, &m_swapchain)) {
			MAGE_THROW(failed to create swapchain)
		}

		//get swapchain images
		vkGetSwapchainImagesKHR(m_device, m_swapchain, &m_swapchain_size, nullptr);
		m_swapchain_images.resize(m_swapchain_size);
		vkGetSwapchainImagesKHR(m_device, m_swapchain, &m_swapchain_size, m_swapchain_images.data());

		m_swapchain_surface_format	= swapchainFormat;
		m_swapchain_extent			= swapchainExtent;
		m_swapchain_present_mode	= swapchainPresentMode;
	}

	void VulkanRHI::createSwapchainImageViews() {
		m_swapchain_image_views.resize(m_swapchain_images.size());
		for (int i = 0; i < m_swapchain_images.size(); ++i) {
			VkImageViewCreateInfo swapchainViewInfo				= VulkanInfo::aboutVkImageViewCeateInfo();

			swapchainViewInfo.image								= m_swapchain_images[i];
			swapchainViewInfo.viewType							= VK_IMAGE_VIEW_TYPE_2D;
			swapchainViewInfo.format							= m_swapchain_surface_format.format;
			swapchainViewInfo.subresourceRange					= VulkanInfo::aboutVkImageSubresourceRange();
			swapchainViewInfo.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
			swapchainViewInfo.subresourceRange.layerCount		= 1;
			swapchainViewInfo.subresourceRange.levelCount		= 1;
			swapchainViewInfo.subresourceRange.baseMipLevel		= 0;
			swapchainViewInfo.subresourceRange.baseArrayLayer	= 0;


			if (VK_SUCCESS != vkCreateImageView(m_device, &swapchainViewInfo, nullptr, &m_swapchain_image_views[i])) {
				MAGE_THROW(failed to create swapchain image views)
			}
		}
	}

	void VulkanRHI::createDepthImage() {
		m_depth_format = VulkanHelper::findDepthFormat(this, { VK_FORMAT_D32_SFLOAT,VK_FORMAT_D32_SFLOAT_S8_UINT,VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

		VulkanHelper::imageCreationHelper(this, m_swapchain_extent.width, m_swapchain_extent.height,
			m_depth_format,
			1,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_depth_image, m_depth_image_memory);

		VkImageViewCreateInfo depthViewInfo = VulkanInfo::aboutVkImageViewCeateInfo();

		depthViewInfo.image = m_depth_image;
		depthViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		depthViewInfo.format = m_depth_format;
		depthViewInfo.subresourceRange = VulkanInfo::aboutVkImageSubresourceRange();
		depthViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		depthViewInfo.subresourceRange.layerCount = 1;
		depthViewInfo.subresourceRange.levelCount = 1;
		depthViewInfo.subresourceRange.baseMipLevel = 0;
		depthViewInfo.subresourceRange.baseArrayLayer = 0;


		if (VK_SUCCESS != vkCreateImageView(m_device, &depthViewInfo, nullptr, &m_depth_image_view)) {
			MAGE_THROW(failed to create depth image views)
		}
	}

	void VulkanRHI::createCommandPools() {
		VkCommandPoolCreateInfo poolInfo = VulkanInfo::aboutVkCommandPoolCreateInfo();
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = m_queue_indices.m_graphics_family.value();

		if (VK_SUCCESS != vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_command_pools[0])) {
			MAGE_THROW(failed to create command pool)
		}

		poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

		if (VK_SUCCESS != vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_command_pools[1])) {
			MAGE_THROW(failed to create command pool)
		}
	}

	void VulkanRHI::createCommandBuffers() {
		m_command_buffers.resize(std::min(m_max_frames, static_cast<uint32_t>(m_swapchain_images.size())));

		VkCommandBufferAllocateInfo allocateInfo = VulkanInfo::aboutVkCommandBufferAllocateInfo();

		allocateInfo.commandPool		= m_command_pools[0];
		allocateInfo.level				= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandBufferCount = static_cast<uint32_t>(m_command_buffers.size());

		if (VK_SUCCESS != vkAllocateCommandBuffers(m_device, &allocateInfo, m_command_buffers.data())) {
			MAGE_THROW(failed to create command buffers)
		}
	}

	void VulkanRHI::createDescriptorPool() {
		std::array<VkDescriptorPoolSize, 11> poolSizes;
		for (int i = 0; i < 11; ++i) {
			poolSizes[i] = VulkanInfo::aboutVkDescriptorPoolSize();
			poolSizes[i].type = static_cast<VkDescriptorType>(i);
			poolSizes[i].descriptorCount = 100;
		}

		VkDescriptorPoolCreateInfo createInfo = VulkanInfo::aboutVkDescriptorPoolCreateInfo();
		createInfo.flags = 0;
		createInfo.maxSets = m_physical_device_properties.limits.maxBoundDescriptorSets;
		createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		createInfo.pPoolSizes = poolSizes.data();

		if (VK_SUCCESS != vkCreateDescriptorPool(m_device, &createInfo, nullptr, &m_descriptor_pool)) {
			MAGE_THROW(failed to create descriptor descriptor pool)
		}
	}

	void VulkanRHI::createSynchronizationPrimitives() {
		m_fences.resize(m_swapchain_image_views.size());
		m_semaphores.resize(m_swapchain_image_views.size());
		m_present_semaphores.resize(m_swapchain_image_views.size());

		for (int i = 0; i < m_fences.size(); ++i) {
			VkFenceCreateInfo fence = VulkanInfo::aboutVkFenceCreateInfo();
			fence.flags = VK_FENCE_CREATE_SIGNALED_BIT;
			if (VK_SUCCESS != vkCreateFence(m_device, &fence, nullptr, &m_fences[i])) {
				MAGE_THROW(failed to create fences)
			}

			VkSemaphoreCreateInfo semaphore = VulkanInfo::aboutVkSemaphoreCreateInfo();
			if (VK_SUCCESS != vkCreateSemaphore(m_device, &semaphore, nullptr, &m_semaphores[i]) ||
				VK_SUCCESS != vkCreateSemaphore(m_device, &semaphore, nullptr, &m_present_semaphores[i])) {
				MAGE_THROW(failed to create semaphores)
			}
		}
	}
	//
	VkResult VulkanRHI::createDebugUtilsMessengerEXT(VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pcreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto vk_createDebugUtilsMessengerEXT =
			(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
		if (vk_createDebugUtilsMessengerEXT != nullptr) {
			return vk_createDebugUtilsMessengerEXT(instance, pcreateInfo, pAllocator, pDebugMessenger);
		}
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void VulkanRHI::clearupSwapchainRelated() {
		//depth resource
		vkDestroyImageView(m_device, m_depth_image_view,nullptr);
		vkDestroyImage(m_device, m_depth_image, nullptr);
		vkFreeMemory(m_device, m_depth_image_memory, nullptr);

		//swapchain imageViews
		for (auto& views : m_swapchain_image_views) {
			vkDestroyImageView(m_device, views, nullptr);
		}
	}

	void VulkanRHI::recreateSwapchain(std::function<void()> pass_recreate) {
		int width{ 0 }, height{ 0 };
		glfwGetFramebufferSize(m_window, &width, &height);
		while (width == 0 or height == 0) {
			glfwGetFramebufferSize(m_window, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(m_device);

		clearupSwapchainRelated();

		createSwapchain(m_swapchain);

		createSwapchainImageViews();

		createDepthImage();

		pass_recreate();
	}
}