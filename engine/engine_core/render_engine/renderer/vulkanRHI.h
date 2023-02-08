#pragma once
#include<engine_core/render_engine/window_system.h>
#include<core/macro.h>

#include<optional>

namespace Mage {

	struct QueueFamilyIndices {
		std::optional<uint32_t> m_graphics_family;
		std::optional<uint32_t> m_present_family;

		inline bool isComplete() const { return m_graphics_family.has_value() && m_present_family.has_value(); }
	};

	struct SwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> present_modes;
	};

	/*
	* VulkanRHI 负责作为硬件抽象接口（考虑是否将helper class作为友元？）。
	* vulkan字段包含instance ，device，以及渲染过程中用来使用的command buffer。其中command pool被分成了两类，分别负责平常正常使用的和短暂存在的。
	* 因为swapchain中包含多个image views，所以底部对应的信号量，command buffer，等都有一个对应，不过该对应并不需要完美的对应swap chain中的图像索引，
	* 因为他们并不真正使用swapchain，而外部framebuffer需要在attachments中引用swapchain中的图像作为color attachment，所以他们需要swapchain中内部的索引。
	* 这就是RHI中包含两个index字段的原因。
	* frame index负责跟swapchain无关的索引，只要保证该帧内的资源可用即可。
	* swapchain index负责让外部framebuffer能够索引到输出的image。
	*/
	class VulkanRHI {
	public:

		static bool m_enable_validation_layers;
		static bool m_enable_debug_utils_label;
		static std::vector<const char*> m_validation_layers;
		static std::vector<const char*> m_device_extensions;

		VulkanRHI() = default;
		~VulkanRHI();

		void initialize(std::shared_ptr<WindowSystem>);
		inline void prepareContext() { m_command_buffer = m_command_buffers[m_current_frame_index]; }

		VkCommandBuffer recordCommandsTemporarily();
		void submitCommandsTemporarily(VkCommandBuffer);

		void waitForFences();

		bool prepareVulkanRHIBeforeRender(std::function<void()> pass_recreate);;
		void prepareVulkanRHIAfterRender();

		inline VkCommandBuffer getCurrentCommandBuffer() { return m_command_buffer; }
		inline uint32_t getCurrentFrameIndex() { return m_current_frame_index; }

		//instance
		inline VkInstance getInstance() { return m_instance; }

		//queue
		inline VkQueue getGraphicsQueue() { return m_graphics_queue; }
		inline VkQueue getPresentQueue() { return m_present_queue; }
		inline uint32_t getGraphicsQueueFamilyIndex() { return m_queue_indices.m_graphics_family.value(); }

		//device
		inline VkDevice getDevice() { return m_device; }
		inline VkPhysicalDevice getPhysicalDevice() { return m_physical_device; }
		inline VkPhysicalDeviceMemoryProperties getPhysicalDeviceMemoryProperties() { return m_physical_device_memory_properties; }
		inline VkPhysicalDeviceFeatures getDeviceSupportFeatures() { return m_physical_device_supported_features; }
		inline VkPhysicalDeviceProperties getDeviceProperties() { return m_physical_device_properties; }
		
		//swapchain
		inline VkSwapchainKHR getSwapchain() { return m_swapchain; }
		inline uint32_t getSwapchainSize() { return m_swapchain_size; }
		inline uint32_t getSwapchainIndex() { return m_swapchain_image_index; }
		inline VkExtent2D getSwapchainExtent() { return m_swapchain_extent; }
		inline VkPresentModeKHR getSwapchainPresentMode() { return m_swapchain_present_mode; }
		inline VkImageView getSwapchainImageView(uint32_t idx) { return m_swapchain_image_views[idx]; }
		inline VkSurfaceFormatKHR getSwapchainSurfaceFormat() { return m_swapchain_surface_format; }
		inline std::vector<VkImageView>& getSwapchainImageViews() { return m_swapchain_image_views; }

		//depth
		inline VkImageView getDepthImageView() { return m_depth_image_view; }
		inline VkFormat getDepthImageFormat() { return m_depth_format; }

		//descriptor pool
		inline VkDescriptorPool getDescriptorPool() { return m_descriptor_pool; }

		//sync
		inline std::vector<VkFence>& getFences() { return m_fences; }
		inline VkFence getFence(uint32_t idx) { return m_fences[idx]; }
		inline std::vector<VkSemaphore>& getPresentSemaphores() { return m_present_semaphores; }
		inline VkSemaphore getPresentSemaphore(uint32_t idx) { return m_present_semaphores[idx]; }
		//TODO: nead more specific name
		inline 	std::vector<VkSemaphore>& getSemaphores() { return m_semaphores; }
		inline VkSemaphore getSemaphore(uint32_t idx) { return m_semaphores[idx]; }

		//others
		inline static const uint32_t getMaxFramesNumber() { return m_max_frames; }
	private:
		//initialize step functions
		void createInstance();
		void setupDebugMessenger();
		void createSurface();
		void createPhysicalDevice();
		void createDevice();
		void createSwapchain(VkSwapchainKHR = VK_NULL_HANDLE);
		void createSwapchainImageViews();
		void createDepthImage();
		void createCommandPools();
		void createCommandBuffers();
		void createDescriptorPool();
		void createSynchronizationPrimitives();

		void recreateSwapchain(std::function<void()> pass_recreate);

		void clearupSwapchainRelated();

		VkResult createDebugUtilsMessengerEXT(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT*, const VkAllocationCallbacks*, VkDebugUtilsMessengerEXT*);
	private:
		STATIC_DATA GLFWwindow*			m_window;
		STATIC_DATA VkInstance			m_instance;

		STATIC_DATA VkSurfaceKHR		m_surface;

		//runtime context
		DYNAMIC_DATA VkCommandBuffer m_command_buffer;

		//device relatives
		STATIC_DATA VkPhysicalDevice	m_physical_device;
		STATIC_DATA VkDevice			m_device;
		STATIC_DATA QueueFamilyIndices  m_queue_indices;
		STATIC_DATA VkQueue				m_graphics_queue; //DONT DESTROY EXPLICITLY
		STATIC_DATA VkQueue				m_present_queue;  //DONT DESTROY EXPLICITLY
		STATIC_DATA VkPhysicalDeviceProperties			m_physical_device_properties;
		STATIC_DATA VkPhysicalDeviceMemoryProperties	m_physical_device_memory_properties;
		STATIC_DATA VkPhysicalDeviceFeatures			m_physical_device_supported_features;
		STATIC_DATA VkPhysicalDeviceFeatures			m_physical_device_required_features;
		STATIC_DATA VkPhysicalDeviceLimits				m_physical_device_limits;

		//swapchain relatives
		DYNAMIC_DATA uint32_t					m_swapchain_image_index;
		DYNAMIC_DATA uint32_t					m_swapchain_size;
		DYNAMIC_DATA VkSwapchainKHR				m_swapchain{ VK_NULL_HANDLE };
		DYNAMIC_DATA VkSurfaceFormatKHR			m_swapchain_surface_format;
		DYNAMIC_DATA VkExtent2D					m_swapchain_extent;
		DYNAMIC_DATA VkPresentModeKHR			m_swapchain_present_mode;
		DYNAMIC_DATA std::vector<VkImage>		m_swapchain_images;
		DYNAMIC_DATA std::vector<VkImageView>	m_swapchain_image_views;

		//depth buffer relatives
		DYNAMIC_DATA VkFormat m_depth_format;
		DYNAMIC_DATA VkImage m_depth_image;
		DYNAMIC_DATA VkImageView m_depth_image_view;
		DYNAMIC_DATA VkDeviceMemory m_depth_image_memory;

		DYNAMIC_DATA uint8_t m_current_frame_index{ 0 };
		STATIC_DATA  static uint32_t const m_max_frames{ 3 };

		//command buffers relatives
		STATIC_DATA std::array<VkCommandPool, 2> m_command_pools; //graphics and transient
		STATIC_DATA std::vector<VkCommandBuffer> m_command_buffers;

		//descriptor pool
		STATIC_DATA VkDescriptorPool m_descriptor_pool;

		//synchronization relatives
		STATIC_DATA std::vector<VkFence> m_fences;
		STATIC_DATA std::vector<VkSemaphore> m_semaphores;
		STATIC_DATA std::vector<VkSemaphore> m_present_semaphores;

		VkDebugUtilsMessengerEXT m_debugMessenger;
	};
}