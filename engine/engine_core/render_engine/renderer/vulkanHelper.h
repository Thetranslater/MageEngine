#pragma once
#include"vulkan/vulkan.h"
#include<glfw-3.3.7.bin.WIN64/include/GLFW/glfw3.h>
#include"engine_core/render_engine/renderer/vulkanRHI.h"

#include<vector>
#include<iostream>
#include<type_traits>

namespace Mage {

	struct QueueFamilyIndices;
	struct SwapchainSupportDetails;
	class VulkanRHI;

	class VulkanHelper {
	public:
		//create Instance
		static bool checkValidationLayerSupport();
		static std::vector<const char*> getRequiredExtensions();
		static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& debugInfo);

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilsCallBack(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		//check physical device suitable or not, surface for present function, features for select suitable device
		static bool isDeviceSuitable(VkPhysicalDevice physical_device, VkPhysicalDeviceFeatures required_features, VkSurfaceKHR surface);
		
		//find a suitable memory type for buffer
		static uint32_t findMemoryType(VulkanRHI* rhi, uint32_t typeFilter, VkMemoryPropertyFlags properties);
		//find a suitable depth format
		static VkFormat findDepthFormat(VulkanRHI* rhi, const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features);
		//TODO:include depth-stencil image layout transition
		//transition image layout
		static void transitionImageLayout(VulkanRHI* rhi, VkImage image, VkImageLayout old_layout, VkImageLayout new_layout, uint32_t miplevels);

		//surface for present
		static QueueFamilyIndices findQueueFamily(VkPhysicalDevice physical_device,VkSurfaceKHR surface);
		static bool checkDeviceExtensionSupport(VkPhysicalDevice physical_device);
		static SwapchainSupportDetails querySwapchainDetails(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
		static bool checkDeviceFeaturesSupport(VkPhysicalDevice physical_device, VkPhysicalDeviceFeatures required_features);
		static VkSurfaceFormatKHR chooseSwapchainSurfaceFormatFromDetails(const std::vector<VkSurfaceFormatKHR>& formats);
		static VkPresentModeKHR chooseSwapchainSurfacePresentModeFromDetails(const std::vector<VkPresentModeKHR>& modes);
		static VkExtent2D chooseSwapchainSurfaceExtentFromDetails(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& surface_capabilities);

		//creation helper
		static void imageCreationHelper(VulkanRHI* rhi, uint32_t width, uint32_t height, VkFormat format, uint32_t mip_levels, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags memory_properties, VkImage& image, VkDeviceMemory& image_memory);
		static void bufferCreationHelper(VulkanRHI* rhi, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& memory);
		static VkShaderModule shaderModuleCreationHelper(VkDevice device, const std::string& filename);
		static void imageViewCreationHelper(VulkanRHI* rhi, VkImage parent, VkFormat format, VkImageAspectFlags aspect, uint32_t mipmaplevel, VkImageView& view);

		//move(or copy) helper
		template<typename Input>
		static void moveDataFromVectorToBuffer(VulkanRHI* rhi, Input start, Input end, VkBuffer& buffer) {
			size_t size{ 0 };
			if constexpr (std::is_pointer<Input>::value) {
				size = sizeof(std::remove_pointer<Input>::type) * (end - start);
			}
			else size = sizeof(Input::value_type) * (end - start);
			VkBuffer staging_buffer;
			VkDeviceMemory staging_memory;

			bufferCreationHelper(rhi, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_memory);

			void* ppdata{ nullptr };

			vkMapMemory(rhi->getDevice(), staging_memory, 0, size, 0, &ppdata);
			memcpy(ppdata, (const void*)&(*start), size);
			vkUnmapMemory(rhi->getDevice(), staging_memory);

			fromBufferToBufferCopyHelper(rhi, staging_buffer, buffer, size);

			vkDestroyBuffer(rhi->getDevice(), staging_buffer, nullptr);
			vkFreeMemory(rhi->getDevice(), staging_memory, nullptr);
		}
		static void fromBufferToBufferCopyHelper(VulkanRHI* rhi, VkBuffer src, VkBuffer dst, VkDeviceSize size);
		static void fromBufferToImageCopyHelper(VulkanRHI* rhi, VkBuffer src, VkImage dst, uint32_t width, uint32_t height);
	};
}