#define VULKAN_INFO 1

#include"core/macro.h"

#include"engine_core/render_system/renderer/vulkanHelper.h"
#include"engine_core/render_system/renderer/vulkanRHI.h"
#include"engine_core/render_system/renderer/vulkanInfo.h"

#include<algorithm>
#include<fstream>
#include<iostream>
#include<set>

namespace Mage {

	bool VulkanHelper::checkValidationLayerSupport() {
		uint32_t layerCount{ 0 };
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> layers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

		for (auto& layer_name : VulkanRHI::m_validation_layers) {
			bool is_find{ false };
			for (auto& layer : layers) {
				if (strcmp(layer.layerName, layer_name) == 0) {
					is_find = true;
					break;
				}
			}
			if (!is_find) {
				return false;
			}
		}
		return true;
	}

	std::vector<const char*> VulkanHelper::getRequiredExtensions() {
		uint32_t glfwExtensionCount{ 0 };
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (VulkanRHI::m_enable_validation_layers || VulkanRHI::m_enable_debug_utils_label) {
			extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	void VulkanHelper::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debugInfo) {
		debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugInfo.messageSeverity = 
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugInfo.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugInfo.pfnUserCallback = debugUtilsCallBack;
		debugInfo.pUserData = nullptr;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanHelper::debugUtilsCallBack(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	QueueFamilyIndices VulkanHelper::findQueueFamily(VkPhysicalDevice physical_device,VkSurfaceKHR surface) {
		QueueFamilyIndices indices;
		uint32_t queueFamiliesCount{ 0 };
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queueFamiliesCount, nullptr);
		std::vector<VkQueueFamilyProperties> queue_family_properties(queueFamiliesCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queueFamiliesCount, queue_family_properties.data());

		int index{ 0 };
		for (auto& properties : queue_family_properties) {
			if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.m_graphics_family = index;
			}
			VkBool32 is_present_support{ false };
			vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, index, surface, &is_present_support);
			if (is_present_support) {
				indices.m_present_family = index;
			}
			if (indices.isComplete()) break;
			++index;
		}
		return indices;
	}

	uint32_t VulkanHelper::findMemoryType(VulkanRHI* rhi, uint32_t typeFilter, VkMemoryPropertyFlags properties) {

		for (int i = 0; i < rhi->getPhysicalDeviceMemoryProperties().memoryTypeCount; ++i) {
			if (typeFilter & (1 << i) && (rhi->getPhysicalDeviceMemoryProperties().memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		MAGE_THROW(failed to find suitable memory type for buffer)
	}

	VkFormat VulkanHelper::findDepthFormat(VulkanRHI* rhi, const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features) {
		for (auto format : formats) {
			VkFormatProperties props;
			//the support of a format cann be queried using....
			vkGetPhysicalDeviceFormatProperties(rhi->getPhysicalDevice(), format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) return format;
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) return format;
		}
		throw std::runtime_error("failed to find support format!");
	}

	bool VulkanHelper::checkDeviceExtensionSupport(VkPhysicalDevice physical_device) {
		uint32_t extensionCount{ 0 };
		vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extension_properties(extensionCount);
		vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensionCount, extension_properties.data());

		std::set<std::string> required_extensions(VulkanRHI::m_device_extensions.begin(), VulkanRHI::m_device_extensions.end());
		for (auto& extension_property : extension_properties) {
			required_extensions.erase(extension_property.extensionName);
		}
		return required_extensions.empty();
	}

	SwapchainSupportDetails VulkanHelper::querySwapchainDetails(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
		SwapchainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.capabilities);

		uint32_t formatCount{ 0 };
		vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &formatCount, nullptr);
		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount{ 0 };
		vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentModeCount, nullptr);
		if (presentModeCount != 0) {
			details.present_modes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentModeCount, details.present_modes.data());
		}
		
		return details;
	}

	bool VulkanHelper::checkDeviceFeaturesSupport(VkPhysicalDevice physical_device, VkPhysicalDeviceFeatures required_features) {
		VkPhysicalDeviceFeatures2 device_features = VulkanInfo::aboutVkPhisicalDeviceFeatures2();
		VkPhysicalDeviceRobustness2FeaturesEXT extension_features = VulkanInfo::abouotVkPhysicalDeviceRobustness2FeaturesEXT();
		extension_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT;
		device_features.pNext = &extension_features;
		vkGetPhysicalDeviceFeatures2(physical_device, &device_features);

		//可能包含对齐BUG？
		VkBool32* p_device_features = reinterpret_cast<VkBool32*>(&device_features);
		VkBool32* p_required_features = reinterpret_cast<VkBool32*>(&required_features);
		for (int i = 0; i < (sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32)); ++i) {
			*(p_device_features + i) &= *(p_required_features + i);
			if (*(p_device_features + i) != *(p_required_features + i))
				return false;
		}
		return true && extension_features.nullDescriptor;
	}

	bool VulkanHelper::isDeviceSuitable(VkPhysicalDevice physical_device, VkPhysicalDeviceFeatures required_features, VkSurfaceKHR surface) {
		auto indices = findQueueFamily(physical_device, surface);

		bool is_features_support = checkDeviceFeaturesSupport(physical_device, required_features);
		bool is_extension_support = checkDeviceExtensionSupport(physical_device);
		bool is_swapchain_adequate{ false };


		if (is_extension_support) {
			auto swapchain_details = querySwapchainDetails(physical_device, surface);
			is_swapchain_adequate = !swapchain_details.formats.empty() && !swapchain_details.present_modes.empty();
		}

		return is_swapchain_adequate && indices.isComplete() && is_features_support;
	}

	VkSurfaceFormatKHR VulkanHelper::chooseSwapchainSurfaceFormatFromDetails(const std::vector<VkSurfaceFormatKHR>& formats) {
		for (auto& format : formats) {
			if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return format;
			}
		}
		return formats[0];
	}

	VkPresentModeKHR VulkanHelper::chooseSwapchainSurfacePresentModeFromDetails(const std::vector<VkPresentModeKHR>& modes) {
		for (auto& mode : modes) {
			if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return mode;
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanHelper::chooseSwapchainSurfaceExtentFromDetails(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& surface_capabilities) {
		if (surface_capabilities.currentExtent.width != UINT32_MAX && surface_capabilities.currentExtent.height != UINT32_MAX) {
			return surface_capabilities.currentExtent;
		}
		else {
			int width{ 0 };
			int height{ 0 };
			glfwGetFramebufferSize(window, &width, &height);
			VkExtent2D extent{ static_cast<uint32_t>(width),static_cast<uint32_t>(height) };
			extent.width	= std::clamp<uint32_t>(extent.width, surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
			extent.height = std::clamp<uint32_t>(extent.height, surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);

			return extent;
 		}
	}

	void VulkanHelper::transitionImageLayout(VulkanRHI* rhi, VkImage image, VkImageLayout old_layout, VkImageLayout new_layout, uint32_t miplevels) {
		VkCommandBuffer command_buffer = rhi->recordCommandsTemporarily();

		VkImageMemoryBarrier barrier = VulkanInfo::aboutVkImageMemoryBarrier();
		barrier.oldLayout = old_layout;
		barrier.newLayout = new_layout;
		barrier.image = image;

		barrier.subresourceRange = VulkanInfo::aboutVkImageSubresourceRange();
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.levelCount = miplevels;
		barrier.subresourceRange.layerCount = 1;

		//determine pipeline stages
		VkPipelineStageFlags source_stage;
		VkPipelineStageFlags destination_stage;

		if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT; //not a real stage in graphics or compute pipeline
		}
		else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			//depth buffer在进行深度测试时读取，发生在early fragment test阶段，而写入则是late fragment test阶段，
			destination_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else {
			MAGE_THROW(unsupported layout transition!)
		}

		vkCmdPipelineBarrier(command_buffer, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		rhi->submitCommandsTemporarily(command_buffer);
	}

	VkShaderModule VulkanHelper::shaderModuleCreationHelper(VkDevice device,const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		
		if (!file.is_open()) {
			MAGE_THROW(failed to open the spir-v file)
		}

		size_t file_size = (size_t)file.tellg();
		std::vector<char> code(file_size);

		file.seekg(0);
		file.read(code.data(), file_size);
		file.close();

		VkShaderModuleCreateInfo createInfo = VulkanInfo::aboutVkShaderModuleCreateInfo();
		createInfo.codeSize = static_cast<uint32_t>(file_size);
		createInfo.pCode = reinterpret_cast<uint32_t*>(code.data());

		VkShaderModule shader_module;
		if (VK_SUCCESS != vkCreateShaderModule(device, &createInfo, nullptr, &shader_module)) {
			MAGE_THROW(failed to create shader module)
		}

		return shader_module;
	}

	void VulkanHelper::imageCreationHelper(VulkanRHI* rhi, uint32_t width, uint32_t height, VkFormat format, uint32_t mip_levels, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memory_properties, VkImage& image, VkDeviceMemory& image_memory) {
		VkImageCreateInfo image_create_info = VulkanInfo::aboutVkImageCreateInfo();
		image_create_info.imageType = VK_IMAGE_TYPE_2D;
		image_create_info.format = format;
		image_create_info.extent = { width,height,1 };
		image_create_info.mipLevels = mip_levels;
		image_create_info.arrayLayers = 1;
		image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_create_info.tiling = tiling;
		image_create_info.usage = usage;
		image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		if (VK_SUCCESS != vkCreateImage(rhi->getDevice(), &image_create_info, nullptr, &image)) {
			MAGE_THROW(failed to create image)
		}

		VkMemoryRequirements image_memory_requires;
		vkGetImageMemoryRequirements(rhi->getDevice(), image, &image_memory_requires);

		VkMemoryAllocateInfo image_allocate_info = VulkanInfo::aboutVkMemoryAllocateInfo();
		image_allocate_info.allocationSize = image_memory_requires.size;
		image_allocate_info.memoryTypeIndex = VulkanHelper::findMemoryType(rhi, image_memory_requires.memoryTypeBits, memory_properties);
		vkAllocateMemory(rhi->getDevice(), &image_allocate_info, nullptr, &image_memory);

		vkBindImageMemory(rhi->getDevice(), image, image_memory, 0);
	}

	void VulkanHelper::bufferCreationHelper(VulkanRHI* rhi, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& memory) {
		VkBufferCreateInfo buffer_create_info = VulkanInfo::aboutVkBufferCreateInfo();
		buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		buffer_create_info.size = size;
		buffer_create_info.usage = usage;

		if (VK_SUCCESS != vkCreateBuffer(rhi->getDevice(), &buffer_create_info, nullptr, &buffer)) {
			MAGE_THROW(failed to create buffer)
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(rhi->getDevice(), buffer, &memRequirements);

		VkMemoryAllocateInfo allocation = VulkanInfo::aboutVkMemoryAllocateInfo();
		allocation.allocationSize = memRequirements.size;
		allocation.memoryTypeIndex = findMemoryType(rhi, memRequirements.memoryTypeBits, properties);

		if (VK_SUCCESS != vkAllocateMemory(rhi->getDevice(), &allocation, nullptr, &memory)) {
			MAGE_THROW(failed to allocate memory for buffer)
		}

		vkBindBufferMemory(rhi->getDevice(), buffer, memory, 0);
	}

	void VulkanHelper::fromBufferToBufferCopyHelper(VulkanRHI* rhi, VkBuffer src, VkBuffer dst, VkDeviceSize size) {
		VkCommandBuffer temp_command_buffer = rhi->recordCommandsTemporarily();

		VkBufferCopy copyRegion = VulkanInfo::aboutVkBufferCopy();
		copyRegion.size = size;
		copyRegion.dstOffset = 0;
		copyRegion.srcOffset = 0;

		vkCmdCopyBuffer(temp_command_buffer, src, dst, 1, &copyRegion);

		rhi->submitCommandsTemporarily(temp_command_buffer);
	}

	void VulkanHelper::fromBufferToImageCopyHelper(VulkanRHI* rhi, VkBuffer src, VkImage dst, uint32_t width, uint32_t height) {
		VkCommandBuffer command_buffer = rhi->recordCommandsTemporarily();

		VkBufferImageCopy copy_region = VulkanInfo::aboutVkBufferImageCopy();
		copy_region.bufferOffset = 0;
		copy_region.imageOffset = { 0,0,0 };

		copy_region.imageSubresource = VulkanInfo::aboutVkImageSubresourceLayers();
		copy_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copy_region.imageSubresource.baseArrayLayer = 0;
		copy_region.imageSubresource.layerCount = 1;
		copy_region.imageSubresource.mipLevel = 0;

		copy_region.imageExtent = { width,height,1 };

		vkCmdCopyBufferToImage(command_buffer, src, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);

		rhi->submitCommandsTemporarily(command_buffer);
	}

	void VulkanHelper::imageViewCreationHelper(VulkanRHI* rhi, VkImage parent, VkFormat format, VkImageAspectFlags aspect, uint32_t mipmaplevel, VkImageView& imageView) {
		VkImageViewCreateInfo view_info = VulkanInfo::aboutVkImageViewCeateInfo();
		view_info.image = parent;
		view_info.format = format;
		view_info.components.a = VK_COMPONENT_SWIZZLE_A;
		view_info.components.r = VK_COMPONENT_SWIZZLE_R;
		view_info.components.g = VK_COMPONENT_SWIZZLE_G;
		view_info.components.b = VK_COMPONENT_SWIZZLE_B;
		view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		view_info.subresourceRange.aspectMask = aspect;
		view_info.subresourceRange.baseArrayLayer = 0;
		view_info.subresourceRange.layerCount = 1;
		view_info.subresourceRange.baseMipLevel = 0;
		view_info.subresourceRange.levelCount = mipmaplevel;

		ASSERT_RESULT(vkCreateImageView(rhi->getDevice(), &view_info, nullptr, &imageView));
	}
}