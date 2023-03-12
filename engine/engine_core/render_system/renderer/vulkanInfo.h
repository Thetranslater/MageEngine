#pragma once
#include<vulkan/vulkan.h>

#ifndef VULKAN_INFO
#define VULKAN_INFO 0
#endif

namespace Mage {
	//vulkan structure information doc
	namespace VulkanInfo {
#if VULKAN_INFO > -1

		/// <summary>
		/// SEPCIFY: x, y, width, height, minDepth, maxDepth.////
		/// Viewport transformation is the final coordinate transform before resterization.
		/// </summary>
		/// <param name="x, y			"> : specify the upper left corner.</param>
		/// <param name="width, height		"> : specify the size of the viewport.</param>
		/// <param name="minDepth, maxDepth	"> : specify the depth range for the viewport.</param>
		inline VkViewport aboutVkViewport() {
			VkViewport viewport{};
			return viewport;
		}

		/// <summary>
		/// SPECIFY:offset, extent.
		/// </summary>
		/// <param name="offset	"> : the offset in pixels within an image or framebuffer.</param>
		/// <param name="extent	"> : the extent of the rectangle.</param>
		inline VkRect2D aboutVkRect2D() {
			VkRect2D rect{};
			return rect;
		}

		/// <summary>
		/// SPECIFY:x, y.
		/// </summary>
		/// <param name="x"> : the x coordinate of the offset.</param>
		/// <param name="y"> : the y coordinate of the offset.</param>
		inline VkOffset2D aboutVkOffset2D() {
			VkOffset2D offset{};
			return offset;
		}

		/// <summary>
		/// SPECIFY:width, height.
		/// </summary>
		/// <param name="width	"> : the width of the extent.</param>
		/// <param name="height	"> : the height of the extent.</param>
		inline VkExtent2D aboutVkExtent2D() {
			VkExtent2D extent{};
			return extent;
		}

		/// <summary>
		/// SPECIFY:srcOffset, dstOffset, size.
		/// </summary>
		/// <param name="srcOffset	"> : the offset of copied region in the source buffer.</param>
		/// <param name="dstOffset	"> : the offset of destination buffer where the copied data sit.</param>
		/// <param name="size		"> : the size of the copy region.</param>
		inline VkBufferCopy aboutVkBufferCopy() {
			VkBufferCopy copy{};
			return copy;
		}

		/// <summary>
		/// SPECIFY:bufferOffset, imageSubresource, imageOffset, imageExtent. OPTIONAL:bufferRowLength, bufferImageHeight.////
		/// You can leave zero to bufferRowLength and bufferImageHeight, command will assume that the buffer is considered to be tightly packed according to the image extent.
		/// </summary>
		/// <param name="bufferOffset		"> : specifies the offset from the start of source buffer where the data is copied from or to.</param>
		/// <param name="bufferRowLength	"> : specifies the number of texels in the image, must greater or equal to width of image extent.</param>
		/// <param name="bufferImageHeight	"> : specifies the number of rows in the image, must greater or equal to height of image extent.</param>
		/// <param name="imageSubresource	"> : specifies the subresource of the image to become source or destination. </param>
		/// <param name="imageOffset		"> : specifies the offset coordinate in texels of image data.</param>
		/// <param name="imageExtent		"> : specifies the size in texels of the image to copy from or to.</param>
		inline VkBufferImageCopy aboutVkBufferImageCopy() {
			VkBufferImageCopy copy{};
			return copy;
		}

		/// <summary>
		/// No fields need to specified explicitly.//// Layers and Extensions are optional, they will cover all devices under the instance.
		/// You can add more informations about you application in 'pApplicationInfo' field, which is a 'VkApplicationInfo' structure.
		/// Also you can edit your debug callback function into 'pNext' field.
		/// </summary>
		/// <param name="pApplicationInfo		"> : pointer to a VkApplicationInfo</param>
		/// <param name="enabledLayerCount		"> : the number of global layers to enable, layers will enable for all devices under the instance</param>
		/// <param name="ppEnabledLayerNames		"> : layers need to enabled</param>
		/// <param name="enabledExtensionCount	"> : the number of global extensions to enable, extensions will enable for all devices under the instance</param>
		/// <param name="ppEnabledExtensionNames	"> : required extensions </param>
		/// <param name="pNext				"> : you can link a VkDebugReportCallbackCreateInfoEXT or a VkDebugUtilsMessengerCreateInfoEXT to it.
		///		it will triggered only for the vkCreateInstance and vkDestroyInstance. </param>
		inline VkInstanceCreateInfo aboutVkInstanceCreateInfo() {
			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.enabledExtensionCount = 0;
			createInfo.enabledLayerCount = 0;
			createInfo.ppEnabledExtensionNames = nullptr;
			createInfo.ppEnabledLayerNames = nullptr;
			createInfo.flags = 0;
			createInfo.pApplicationInfo = nullptr;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY: engineVersion,apiVersion,applicationVersion,pEngineName,pApplicationName. OPTIONAL: you can add names about the application
		/// </summary>
		/// <param name="applicationVersion	"> : developer supplied application version.Use VK_MAKE_VERSION macro. </param>
		/// <param name="engineVersion		"> : developer supplied engine version.Use VK_MAKE_VERSION macro. </param>
		/// <param name="apiVersion		"> : must be the highest version that application designed to use.Use VK_API_VERSION_{} macro.</param>
		/// <param name="pEngineName		"> : the name of the engine.</param>
		/// <param name="pApplicationName	"> : the name of the application.</param>
		inline VkApplicationInfo aboutVkApplicationInfo() {
			VkApplicationInfo info{};
			info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			info.pNext = nullptr;
			info.pEngineName = VK_NULL_HANDLE;
			info.pApplicationName = VK_NULL_HANDLE;
			return info;
		}

		/// <summary>
		/// SPECIFY: extensions, queueCreateInfos. OPTIONAL: features.////
		/// In general, you always need a SWAPCHAIN extension, so the extension fields can't be NULL.
		/// The features that device needs to enabled are depend on you. And you need to specify which queues that the device are going to use 
		/// </summary>
		/// <param name="enabledExtensionCount	"> : the number of the extension to enable, extensions will use only for the specified device.</param>
		/// <param name="ppEnabledExtensionNames	"> : the names of enabled extensions.</param>
		/// <param name="ppEnabledFeatures		"> : the features that the device enabled, use vkGetPhysicalDeviceFeatures to check the features that the physical device support</param>
		/// <param name="queueCreateInfoCount		"> : the number of queue families that device request.</param>
		/// <param name="pQueueCreateInfos		"> : pointer to an array of ''VkDeviceQueueCreateInfo'</param>
		inline VkDeviceCreateInfo aboutVkDeviceCreateInfo() {
			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.enabledLayerCount = 0; //deprecated
			createInfo.ppEnabledLayerNames = nullptr; //deprecated
			return createInfo;
		}

		/// <summary>
		/// SPECIFY: queueFamilyIndex, queueCount. OPTIONAL: pQueuePriorities.
		/// </summary>
		/// <param name="queueFamilyIndex	"> : index of the array returned by vkGetPhysicalDeviceQueueFamilyProperties.Indicate the queue family created on the device</param>
		/// <param name="queueCount		"> : the number of queues of the queueFamilyIndex.It is limited by the corresponding VkQueueFamilyProperties structure</param>
		/// <param name="pQueuePriorities	"> : pointer to an array of floating point values representing the relative priority of work submitted to each of the queues.
		///		Values are normarlized in the range of 0 to 1. </param>
		inline VkDeviceQueueCreateInfo aboutVkDeviceQueueCreateInfo() {
			VkDeviceQueueCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			createInfo.pNext = nullptr;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY:surface, minImageCount, imageFormat, imageColorSpace, imageExtent, imageUsage, imageSharingMode, queueFamilyIndices, presentMode, preTransform, compositionAlpha.
		/// DEFAULT: clipped-VK_TRUE, oldSwapchain-VK_NULL_HANDLE, imageArrayLayers-1.////
		/// You almost need to describe everything about the images in the swapchain, such as the format, extent, used by queues, presentmode, etc.
		/// </summary>
		/// <param name="surface			"> : the surface used to present.</param>
		/// <param name="minImageCount		"> : the minimum number of images that the swapchain contain.Limited by VkSurfaceCapabilitiesKHR.maxImageCount</param>
		/// <param name="imageFormat			"> : the format of the swapchain images, must be a format which have presentation capability.</param>
		/// <param name="imageColorSpace		"> : if ignore the extensions, the only option is VK_COLORSPACE_SRGB_NONLINEAR_KHR.</param>
		/// <param name="imageExtent			"> : the size(in pixels) of the swapchain images.Limited by VkSurfaceCapabilities.</param>
		/// <param name="imageArrayLayers		"> : specifies the number of layers in each images.</param>
		/// <param name="imageUsage			"> : specify how the images will be used(in addition to as present sources).</param>
		/// <param name="imageSharingMode		"> : sprecifies how the images are to be shared across queues.If set to VK_SHARING_MODE_EXCLUSIVE, the queueFamily part will ignored</param>
		/// <param name="queueFamilyIndexCount	"> : the number of the queue families that access to the images.</param>
		/// <param name="pQueueFamilyIndices		"> : pointer to an array of queue family indices.</param>
		/// <param name="preTransform			"> : specifies how the images should be transformed to the user.Limited by VkSurfaceCapabilitiesKHR.</param>
		/// <param name="compositeAlpha		"> : controls how alpha composition is handled.</param>
		/// <param name="presentMode			"> : detemines the way to present between window and render engine.</param>
		/// <param name="clipped			"> : optimize option, better to set to VK_TRUE, means that vulkan does not render the area that invisible.</param>
		/// <param name="oldSwapChain			"> : for recreate swapchain.</param>
		inline VkSwapchainCreateInfoKHR aboutVkSwapchainCreateInfoKHR() {
			VkSwapchainCreateInfoKHR createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.pNext = nullptr;
			createInfo.imageArrayLayers = 1;
			createInfo.clipped = VK_TRUE;
			createInfo.oldSwapchain = VK_NULL_HANDLE;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY:allocationSize, memoryTypeIndex.////
		/// Normally, you use vkGet*MemoryRequirements to get a VkMemoryRequirements strucure to help you allocate device memory.You can find all details in that structure when you populate this structure.
		/// </summary>
		/// <param name="allocationSize		"> : the size of the allocation in bytes.</param>
		/// <param name="memoryTypeIndex	"> : an index of the memoryTypes array of the VkPhysicalDeviceMemoryProperties.</param>
		inline VkMemoryAllocateInfo aboutVkMemoryAllocateInfo() {
			VkMemoryAllocateInfo allocation{};
			allocation.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocation.pNext = nullptr;
			return allocation;
		}

		/// <summary>
		/// SPECIFY:size, usage, sharingMode, queueFamilyIndices.
		/// OPTIONAL:flags.////
		/// The information when you create a resource buffer.Buffer is a linear resource in the vulkan.
		/// </summary>
		/// <param name="size				"> : specifies the size of the buffer, in bytes.</param>
		/// <param name="usage				"> : tells vulkan how you're going to use the buffer.</param>
		/// <param name="sharingMode			"> : indicates that whether	the buffer is accessed by a single queue or potentially accessed by multiply queues.</param>
		/// <param name="queueFamilyIndexCount	"> : the number of the queues.</param>
		/// <param name="pQueueFamilyIndices	"> : when sharingMode is set to VK_SHARING_MODE_CONCURRENT, you need to tell vulkan which queues are going to use the buffer.</param>
		inline VkBufferCreateInfo aboutVkBufferCreateInfo() {
			VkBufferCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			createInfo.pNext = nullptr;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY:imageType, format, extent, tilingMode, usage, sharingMode, queueFamilyIndexCount, pQueueFamilyIndices, initialLayout.
		/// OPTIONAL:flags, mipLevels, arrayLayers, samples.
		/// DEFAULT:mipLevels-1, arraylayers-1, samples-VK_SAMPLE_COUNT_1_BIT.////
		/// The extent field includes width, height and depth.For 1D image, height and depth both set to 1.For 2D image, depth sets to 1.
		/// </summary>
		/// <param name="imageType			"> : options are VK_IMAGE_TYPE_{1D,2D,3D}.</param>
		/// <param name="format			"> : the format of the image.</param>
		/// <param name="extent				"> : the size of the image in texels.</param>
		/// <param name="mipLevels			"> : the number of mipmap levels to create in the image.The base level is 0, and it has the highest resolution.</param>
		/// <param name="arrayLayers			"> : you can imagine that some images grouped into a single image object, so this field indicate that how many images does the object have.</param>
		/// <param name="samples			"> : for multisamples when the image used as a attachment.</param>
		/// <param name="tiling				"> : specifies how does the underlying layout of the image in memory. Usually, choose optimal mode.</param>
		/// <param name="usage				"> : describing where the image will be used.</param>
		/// <param name="sharingMode			"> : indicates that whether	the image is accessed by a single queue or potentially accessed by multiply queues.</param>
		/// <param name="queueFamilyIndexCount	"> : if sharingMode is VK_SHARING_MODE_CONCURRENT, the field specifies the number of the queues that will access this image.</param>
		/// <param name="pQueueFamilyIndices		"> : if sharingMode is VK_SHARING_MODE_CONCURRENT, the field specifies which queues will access this image.</param>
		/// <param name="initialLayout			"> : the layout when the image created.</param>
		inline VkImageCreateInfo aboutVkImageCreateInfo() {
			VkImageCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			createInfo.pNext = nullptr;
			//default
			createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			createInfo.mipLevels = 1;
			createInfo.arrayLayers = 1;

			return createInfo;
		}

		/// <summary>
		/// SPECIFY:buffer, format, offset, range.
		/// </summary>
		/// <param name="buffer	"> : the buffer which the view bind to.</param>
		/// <param name="format"> : decribing the format of data in the buffer.</param>
		/// <param name="offset	"> : an offset in bytes, specifies the start address of the view.</param>
		/// <param name="range	"> : a size in bytes of the buffer view.</param>
		inline VkBufferViewCreateInfo aboutVkBufferViewCreateInfo() {
			VkBufferViewCreateInfo create_info{};
			create_info.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
			create_info.pNext = nullptr;
			return create_info;
		}

		/// <summary>
		/// SPECIFY:image, viewType, format, components, subresourceRange.////
		/// The image cannot be used directly, we need more information.
		/// </summary>
		/// <param name="image			"> : the parent image.</param>
		/// <param name="viewType		"> : the viewType should be compatible with the image type.</param>
		/// <param name="format		">: the format of the view. And also must be compatible with the format of parent image.</param>
		/// <param name="components		"> : allows you remapping the component of a format.</param>
		/// <param name="subresourceRange	"> : you can only view a subset of the parent image. </param>
		inline VkImageViewCreateInfo aboutVkImageViewCeateInfo() {
			VkImageViewCreateInfo imageView{};
			imageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageView.pNext = nullptr;
			imageView.flags = 0;
			return imageView;
		}

		/// <summary>
		/// SPECIFY:aspectMask, baseArrayLayer, layerCount, mipLevel.////
		/// The structure usually used for data copy.
		/// </summary>
		/// <param name="aspectMask		"> : for some images have more than one logical part, you can copy the subpart of the data using aspect mask.</param>
		/// <param name="baseArrayLayer	"> : specifies the beginning index of array images.</param>
		/// <param name="layerCount		"> : Starting from the specified index, means that the number of layers to copy.</param>
		/// <param name="mipLevel		"> : specifies the mipmap level to copy.</param>
		inline VkImageSubresourceLayers aboutVkImageSubresourceLayers() {
			VkImageSubresourceLayers subLayers{};
			return subLayers;
		}

		/// <summary>
		/// SPECIFY: aspectMask, baseMipLevel, levelCount, baseArrayLayer, layerCount.////
		/// </summary>
		/// <param name="aspectMask		"> : for some images have more than one logical part, you can fetch the subpart of the data using aspect mask.</param>
		/// <param name="baseMipLevel		"> : specifies the beginning index of mipmaps.</param>
		/// <param name="levelCount		"> : Starting from the specified index, the field means the number of the mipmaps that the subresource has.</param>  
		/// <param name="baseArrayLayer	"> : specifies the beginning index of array images.</param>
		/// <param name="layerCount		"> : Starting from the specified index, means that the number of layers that the subresource has.</param>
		inline VkImageSubresourceRange aboutVkImageSubresourceRange() {
			VkImageSubresourceRange subRange{};
			return subRange;
		}

		/// <summary>
		/// SPECIFY: codeSize, pCode.////
		/// A SPIR-V module should handed into a shader module, all you need to do is fill the size field and the code pointer.
		/// </summary>
		/// <param name="codeSize	"> : the size of the module, in bytes.</param>
		/// <param name="pCode	"> : pointer to the code.Because the pCode point to an array of uint32_t, so you need use reinterpret_cast<uint32_t*> on your byte code pointer.</param>
		inline VkShaderModuleCreateInfo aboutVkShaderModuleCreateInfo() {
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			return createInfo;
		}

		//TODO:Specilization Constants

		/// <summary>
		/// SPECIFY:stage, module, pName.
		/// OPTIONAL:pSpecializationInfo.
		/// </summary>
		/// <param name="stage			"> : the stage that shader used.</param>
		/// <param name="module		"> : a pointer to a shader module.</param>
		/// <param name="pName		"> : specifies the entry point of the shader code.</param>
		/// <param name="pSpecializationInfo	"> : </param>
		inline VkPipelineShaderStageCreateInfo aboutVkPipelineShaderStageCreateInfo() {
			VkPipelineShaderStageCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY: bindingCount, pBindings.////
		/// you need to describe the resources informations that grouped into the descriptor set.That's called DescriptorSetLayout.
		/// </summary>
		/// <param name="bindingCount	"> : every resource need a binding number so that the shader could find it.
		///		The bindingCount not only specifies the size of the pBindings array, but also indicate the corresponding binding number of the resource.</param>
		/// <param name="pBindings	"> : every binding has a resource. An description array about resource is placed here.</param>
		inline VkDescriptorSetLayoutCreateInfo aboutVkDescriptorSetLayoutCreateInfo() {
			VkDescriptorSetLayoutCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			createInfo.pNext = nullptr;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY:binding, descriptorType, descriptorCount, stageFlags.
		/// OPTIONAL:pImmutableSamplers.
		/// DEFAULT:stageFlags-VK_SHADER_STAGE_ALL, descriptorCount-1.////
		/// When the descriptor type is one of two that VK_DESCRIPTOR_TYPE_SAMPLER and VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, then you can use the pImmutableSamplers.
		/// </summary>
		/// <param name="binding		"> : the binding point corresponding to the shader.</param>
		/// <param name="stageFlags		"> : the shader stage which the descriptor set used in.</param> 
		/// <param name="descriptorType	"> : the type of the resource.</param>
		/// <param name="descriptorCount	"> : the number of descritors that the binding has.Always 1, except that the shader has an resource array of the type.</param>
		/// <param name="pImmutableSamplers	"> : a pointer to an array of samplers.</param>
		inline VkDescriptorSetLayoutBinding aboutVkDescriptorSetLayoutBinding() {
			VkDescriptorSetLayoutBinding binding{};
			binding.stageFlags = VK_SHADER_STAGE_ALL;
			binding.pImmutableSamplers = nullptr;
			binding.descriptorCount = 1;
			return binding;
		}

		/// <summary>
		/// SPECIFY: offset, size, stageFlags.
		/// </summary>
		/// <param name="stageFlags	"> : specify the shader stage that use the push-constant data.</param>
		/// <param name="offset		"> : the offset of the push constant data(most zero).</param>
		/// <param name="size		"> : the size of the push-constant use.</param>
		inline VkPushConstantRange aboutVkPushConstantRange() {
			VkPushConstantRange push_constant{};
			push_constant.offset = 0;
			return push_constant;
		}


		/// <summary>
		/// SPECIFY:setLayoutCount, pSetLayouts.
		/// OPTIONAL:pushConstantRangeCount, pPushConstantRanges.////
		/// </summary>
		/// <param name="setLayoutCount		">: the number of the descriptorSetLayout.</param>
		/// <param name="pSetLayouts			"> : a pointer to an array of  descriptorSetLayout structures.</param> 
		/// <param name="pushConstantRangeCount	"> : about push constant.</param>
		/// <param name="pPushConstantRanges		"> : about push constant.</param>
		inline VkPipelineLayoutCreateInfo aboutVkPipelineLayoutCreateInfo() {
			VkPipelineLayoutCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.pushConstantRangeCount = 0;
			createInfo.pPushConstantRanges = nullptr;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY: flags, maxSets, poolSizeCount, pPoolSizes. 
		/// </summary>
		/// <param name="flags		"> : control the allocation strategy.</param>
		/// <param name="maxSets	"> : the maximum number of descriptor sets that allocated from the pool.</param>
		/// <param name="poolSizeCount"> : the size of the pPoolSize array.</param>
		/// <param name="pPoolSize	"> : a pointer to an array of VkDescriptorPoolSize, which contain the information about the resources types.</param>
		inline VkDescriptorPoolCreateInfo aboutVkDescriptorPoolCreateInfo() {
			VkDescriptorPoolCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			createInfo.pNext = nullptr;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY: type, descriptorCount.
		/// </summary>
		/// <param name="type			"> : type of a specific resource that could allocated from the pool.</param>
		/// <param name="descriptorCount	"> : the total number of descritors of this type to be allocated from the pool.</param>
		inline VkDescriptorPoolSize aboutVkDescriptorPoolSize() {
			VkDescriptorPoolSize size{};
			return size;
		}

		/// <summary>
		/// SPECIFY: descriptorPool, descriptorSetCount, pSetLayouts.
		/// </summary>
		/// <param name="descriptorPool	"> : the pool used to allocate descriptor sets.</param>
		/// <param name="descriptorCount	"> : the number of descriptor sets that you want to allocate.</param>
		/// <param name="pSetLayouts		"> : the layout of each descriptor set.</param>
		inline VkDescriptorSetAllocateInfo aboutVkDescriptorSetAllocateInfo() {
			VkDescriptorSetAllocateInfo allocateInfo{};
			allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocateInfo.pNext = nullptr;
			return allocateInfo;
		}

		/// <summary>
		/// SPRCIFY: dstSet, dstBinding, descriptorType, one of the type info.
		/// OPTIONAL: dstArrayElement, descriptorCount, pImageInfo, pBufferInfo, pTexelBufferView.
		/// DEFAULT: dstArrayElement-0, descriptorCount-1.////
		/// the descriptorType determines the next parameters which one of three that include pImageInfo, pBufferInfo and pTexelBufferView. 
		/// </summary>
		/// <param name="dstSet			"> : the destination set that needs to wrote.</param>
		/// <param name="dstBinding		"> : the binding that the resources bound to.</param>
		/// <param name="dstArrayElement	"> : the beginning index of the update.If the binding need an array resources, use this. Otherwise, take 0.</param>
		/// <param name="descriptorCount	"> : the number of resources that need to updated after the beginning index.</param>
		/// <param name="descriptorType	"> : the type of resources.</param>
		/// <param name="pImageInfo		"> : when the resources are images, this field will considered.</param>
		/// <param name="pBufferInfo		"> : when the resources ara buffers ,this field will considered.</param>
		/// <param name="pTexelBufferView	"> : when the resources are buffer views, this field will considered.</param>
		inline VkWriteDescriptorSet aboutVkWriteDescriptorSet() {
			VkWriteDescriptorSet write{};
			write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			write.pNext = nullptr;
			write.descriptorCount = 1;
			write.dstArrayElement = 0;
			return write;
		}

		/// <summary>
		/// SPECIFY: imageView, imageLayout.
		/// OPTIONAL: sampler.////
		/// when the resource type is VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, you can specify the sampler at the sampler field.
		/// </summary>
		/// <param name="sampler	"> : the sampler used to sample this image.</param>
		/// <param name="imageView	"> : the image view that is going to bound to the set.</param>
		/// <param name="imageLayout	"> : the layout that the image is expected to be in when it is used.</param>
		inline VkDescriptorImageInfo aboutVkDescriptorImageInfo() {
			VkDescriptorImageInfo image{};
			return image;
		}

		/// <summary>
		/// SPECIFY: buffer, offset, range.
		/// </summary>
		/// <param name="buffer"> : the buffer resource.</param>
		/// <param name="offset"> : the offset of the binding.And has different alignment requirements depending on the type of the buffer.
		///		Check VkPhysicalDeviceLimits.</param>
		/// <param name="range"> : the size of the binding.Use VK_WHOLE_SIZE to bind the whole buffer.</param>
		inline VkDescriptorBufferInfo aboutVkDescriptorBufferInfo() {
			VkDescriptorBufferInfo buffer{};
			return buffer;
		}

		/// <summary>
		/// SPECIFY: format, loadOp, storeOp, stencilLoadOp, stencilStoreOp, initialLayout, finalLayout.
		/// OPTIONAL: flags, samples.
		/// DEFAULT: flags-0, samples-VK_SAMPLE_COUNT_1_BIT.////
		/// samples is used of multisampline.And the 'Op' fields indicate the usage at the beginning and the end of the renderpass.
		/// </summary>
		/// <param name="flags		">: the only option is VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT, which means that the attachment might be using the same
		///		memory as another attachment in the same renderpass.Use for memory optimize.</param>
		/// <param name="format	"> : should match the format of the image used as the attachment.</param>
		/// <param name="samples	"> : for multisampling.</param>
		/// <param name="loadOp	"> : how to deal with the attachment when the renderpass begin.</param>
		/// <param name="storeOp	"> : how to deal with the attachment when the renderpass end.</param>
		/// <param name="stencilLoadOp	"> : if attachment is a depth-stencil attachment, you should tell vulkan want to do with the stencil part when the renderpass begin.
		///		(depth part specified in loadOp).</param>
		/// <param name="stencilStoreOp"> : likewise, how to deal with the stencil part when renderpass end.</param>
		/// <param name="initialLayout	"> : the image layout expected by the renderpass when its begins.Note, renderpass do not automatically perform the layout transitions.</param>
		/// <param name="finalLayout	"> : the image layout expected by the renderpass when its ends.Note, renderpass will perform the layout transitions at the end.</param>
		inline VkAttachmentDescription aboutVkAttachmentDescription() {
			VkAttachmentDescription attachment{};
			attachment.samples = VK_SAMPLE_COUNT_1_BIT;
			attachment.flags = 0;
			return attachment;
		}

		/// <summary>
		/// SPECIFY:pipelineBindPoint, inputAttachmentCount, pInputAttachments, colorAttachmentCount, pColorAttachments, pResolveAttachments, pDepthStencilAttachment,
		/// preserveAttachmentCount, pPreserveAttachments.////
		/// Every subpass references attachments as input or output.The difference between input attachments and color attachments is that the input attachments are used for input, the other used for output. 
		/// </summary>
		/// <param name="pipelineBindPoint		"> : this field specifies the pipeline type for this subpass.</param>
		/// <param name="inputAttachmentCount	"> : the number of the input attachments.</param>
		/// <param name="pInputAttachments		"> : a pointer to a VkAttachmentReference array.</param>;
		/// <param name="colorAttachmentCount	"> : the number of the color attachments.</param>
		/// <param name="pColorAttachments		"> : same as pInpuAttachments.</param>
		/// <param name="pResolveAttachments		"> : the number of the resolve attachments is same as the color attachments.It assumes that every color attachment has a resolve attachment.
		/// The resolve attachments used for multissample image, to store the image data resolved.</param>
		/// <param name="pDepthStencilAttachment	"> : a single subpass can only has a single depth-stencil attachment.</param>
		/// <param name="preserveAttachmentCount	"> : the number of the preserve attachments.</param>
		/// <param name="pPreserveAttachments		"> : there ara attachments that not directly referenced by the subpass, but you do want the attachments to live across the subpass.</param>
		inline VkSubpassDescription aboutVkSubpassDescription() {
			VkSubpassDescription subpass{};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.flags = 0;
			return subpass;
		}

		/// <summary>
		/// SPECIFY: attachment, layout.
		/// </summary>
		/// <param name="attachment	"> : a index of the attachment in the renderpassCreateInfo.</param>
		/// <param name="layout		"> : the image layout expected by this subpass.</param>
		inline VkAttachmentReference aboutVkAttachmentReference() {
			VkAttachmentReference attachment_ref{};
			return attachment_ref;
		}

		/// <summary>
		/// SPECIFY:srcSubpass, dstSubpass, srcStageMask, dstStageMask, srcAccessMask, dstAccessMask,
		/// </summary>
		/// <param name="srcSubpass		"> : use index to specify the source subpass that pruduce data.</param>
		/// <param name="dstSubpass		"> : use index to specify the destination subpass that consume data.</param>
		/// <param name="srcStageMask		"> : specifies pipeline stages that produced the data in source subpass.</param>
		/// <param name="dstStageMask		"> : specifies pipeline stages that consumed the data in destinaiton subpass.</param>
		/// <param name="srcAccessMask	"> : specifies that how source subpass access the data.</param>
		/// <param name="dstAccessMask	"> : specifies that how destinaion subpass acess the data.</param> 
		inline VkSubpassDependency aboutVkSubpassDependency() {
			VkSubpassDependency dependency{};
			return dependency;
		}

		/// <summary>
		/// SPECIFY:attachmentCount, pAttachments, subpassCount, pSubpasses, dependencyCount, pDependencies.////
		/// Attachments are resources that used as input or output.Subpasses correspond to pipelines.And dependencies define the order of subpasses.
		/// </summary>
		/// <param name="attachmentCount	"> : the number of attachments this renderpass has.</param>
		/// <param name="pAttachments		"> : a pointer to a VkAttachmentDescription array.</param>
		/// <param name="subpassCount		"> : the number of subpasses this renderpass has.</param>
		/// <param name="pSubpasses		"> : a pointer to a VkSubpassDescription array.</param>
		/// <param name="dependencyCount	"> : the number of dependencies.</param>
		/// <param name="pDependencies	"> : a pointer to a VkSubpassDependency array.</param>
		inline VkRenderPassCreateInfo aboutVkRenderPassCreateInfo() {
			VkRenderPassCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY: renderPass, attachmentCount, pAttachments, width, height, layers.
		/// </summary>
		/// <param name="renderPass		"> : the renderpass that is compatible with the framebuffer.</param>
		/// <param name="attachmentCount	"> : the number of the attachments. And the value must be equal to the attachment count specified in renderpass.</param>
		/// <param name="pAttachments		"> : a pointer to a VkImageView array.</param>
		/// <param name="width			"> : the width of the framebuffer.Limited by the physical device.</param>
		/// <param name="height			"> : the height of the framebuffer.Limited by the physical device.</param>
		/// <param name="layers			"> : the number of layers of the framebuffer.Limited by the physical device.</param>
		inline VkFramebufferCreateInfo aboutVkFramebufferCreateInfo() {
			VkFramebufferCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			return createInfo;
		}

		/// <summary>
		/// Because the pipeline object in vulkan is a "precompile" object, you need to specify a lot of fields in the structure.That's help vulkan to do some optimize operations on pipeline.
		/// Some states in pipeline are optional, however, as this structure is large and complex, the explanation of all fileds is given here.
		/// </summary>
		/// <param name="flags			"> : determines that the pipeline is un-optimized or derivates from the last pipeline creation call or becomes the parent of the pipeline that will be created subsequently.</param>
		/// <param name="stageCount		"> : the number of the shaders contained in pipeline.</param>
		/// <param name="pStages		"> : VkPipelineShaderStageCreateInfo, includes the shaders' code.</param>
		/// <param name="pVertexInputStage	"> : VkPipelineVertexInputStateCreateInfo, describes the vertex input data and includes the bindings and attributes information.</param>
		/// <param name="pInputAssemblyState	"> : VkPipelineInputAssemblyStateCreateInfo, this state will group vertex datas into primitives, so you need to declare the type of primitives here.</param>
		/// <param name="pTessellationState	"> : VkPipelineTessellationStateCreateInfo, tessellation will only be applied to the patch primitives.When you enable this, you need to not only specify the primitives type to the patch type, but also specify the number of vertices in a single patch here. </param>
		/// <param name="pViewportState	"> : VkPipelineViewportStateCreateInfo, the viewport transform is the last transform before rasterization.Specify the viewport size here, and you can use multiple viewports in a single pipeline.</param>
		/// <param name="pRasterizationState	"> : VkPipelineRasterizationStateCreateInfo, configue the rasterizer for some operations.Such as depth clamp, visual winding mode.</param>
		/// <param name="pMultisampleState	"> : VkPipelineMultisampleStateCreateInfo, multisampling is the process of generating multiple samples for each pixel.When use multisampling, the color and depth-stencil attachments must be multisample images.</param>
		/// <param name="pDepthStencilState	"> : VkPipelineDepthStencilStateCreateInfo, depth-stencil state controls how the depth and stencil tests performed.</param> 
		/// <param name="pColorBlendState	"> : VkPipelineColorBlendStateCreateInfo, color blend state controls how the outputs of the fragment shader is wrote into color attachment.</param>
		/// <param name="pDynamicState	"> : VkPipelineDynamicStateCreateInfo, specify the states which will be dynamic.You can use dynamic commands to change state when that state is dynamic.</param>
		/// <param name="layout			"> : the description of binding locations used by both pipeline and descriptor sets used with the pipeline.</param>
		/// <param name="renderpass		"> : the renderpass that this pipeline attached.</param>
		/// <param name="subpass		"> : index of the subpass in the renderpass where pipeline will be used.</param>
		/// <param name="basePipelineHandle	"> : the pipeline to derive from.</param>
		/// <param name="basePipelineIndex	"> : index into the pCraeteInfos parameter in vkCreate*Pipelines to sepcify the pipeline to derive from.</param>
		inline VkGraphicsPipelineCreateInfo aboutVkGraphicsPipelineCreateInfo() {
			VkGraphicsPipelineCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			createInfo.pNext = nullptr;
			//default
			createInfo.flags = 0;
			createInfo.pTessellationState = nullptr;
			createInfo.pMultisampleState = nullptr;
			createInfo.pDynamicState = nullptr;

			return createInfo;
		}

		/// <summary>
		/// SPECIFY:vertexBindingDescriptionCount, pVertexBindingDescriptions, vertexAttributeDescriptionCount, pVertexAttributeDescription.////
		/// Every binding description associate with a vertex buffer, it specifies a binding number so that vulkan can recognize the buffer to read data from.
		/// Attribute descriptions give the information about the vertex data, each of them describes a attribute of the vertex. 
		/// </summary>
		/// <param name="vertexBindingDescriptionCount	"> : the number of the binding descriptions.</param>
		/// <param name="pVertexBindingDescriptions		"> : a pointer to a VkVertexInputBindingDescription array.</param>
		/// <param name="vertexAttributeDescriptionCount	"> : the number of the attributes the a vertex has.</param>
		/// <param name="pVertexAttributeDescription		"> : a pointer to a VkVertexInputAttributeDescription array.</param>
		inline VkPipelineVertexInputStateCreateInfo aboutVkPipelineVertexInputStateCreateInfo() {
			VkPipelineVertexInputStateCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY:binding, stride, inputRate.////
		/// The stride value must match the size of the vertex. 
		/// </summary>
		/// <param name="binding	"> : the binding number of the vertex buffer.Limited by maxVertexInputBindings</param>
		/// <param name="stride		"> : the distance between the start of each vertex.Limited by maxVertexInputBindingStride.</param>
		/// <param name="inputRate	"> : VK_VERTEX_INPUT_RATE_VERTEX or VK_VERTEX_INPUT_RATE_INSTANCE.</param>
		inline VkVertexInputBindingDescription aboutVkVertexInputBindingDescription() {
			VkVertexInputBindingDescription binding{};
			return binding;
		}

		/// <summary>
		/// SPECIFY: location, binding, format, offset.
		/// </summary>
		/// <param name="location	"> : The location used to refer to attribute in the vertex shader.</param>
		/// <param name="binding	"> : the binding sepcifies the bindings in the vertexBindingDescriptions in vertexInputState.</param>
		/// <param name="format	"> : give the format of the attribute.</param>
		/// <param name="offset		"> : give the offset of the attribute in vertex.Limited by maxVertexInputAttributeOffset.</param>
		inline VkVertexInputAttributeDescription aboutVkVertexInputAttributeDescription() {
			VkVertexInputAttributeDescription attribute{};
			return attribute;
		}

		/// <summary>
		/// SPECIFY:topology, primitiveRestartEnable.////
		/// Restarts take effect only when indexed draws.
		/// </summary>
		/// <param name="topology		"> : members of the VkPrimitiveTopology.Specifies the primitive topology.</param>
		/// <param name="primitiveRestartEnable	"> : determines that enable the primitive restart function or not.
		///		This flag used to allow strip and fan primitive topologies to be cut and restarted.</param>
		inline VkPipelineInputAssemblyStateCreateInfo aboutVkPipelineInputAssemblyStateCreateInfo() {
			VkPipelineInputAssemblyStateCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY:: patchControlPoints.////
		/// When topology field of the input assembly creation structure is NOT set to VK_PRIMITIVE_TOPOLOGY_PATCH_LIST, the pTessellationState must be a null pointer.
		/// </summary>
		/// <param name="patchControlPoints"> : the number of control points grouped into a primitive.</param>
		inline VkPipelineTessellationStateCreateInfo aboutVkPipelineTessellationStateCreateInfo() {
			VkPipelineTessellationStateCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY:viewportCount, pViewports, scissorCount, pScissors.////
		/// The scissorCount must be the same as viewportCount.
		/// </summary>
		/// <param name="viewportCount	"> : the number of viewports in pipeline.</param>
		/// <param name="pViewports		"> : the defination of viewports.</param>
		/// <param name="scissorCount		"> : the number of scissor rectangles in pipeline.</param>
		/// <param name="pScissors		"> : the defination of scissor rectangles.</param>
		inline VkPipelineViewportStateCreateInfo aboutVkPipelineViewportStateCreateInfo() {
			VkPipelineViewportStateCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY:polygonMode, cullMode, frontFace.
		/// OPTIONAL:depthClampEnable, rasterizerDiscardEnable, depthBiasEnable, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor, lineWidth.
		/// DEFAULT:polygonMode-VK_POLYGON_MODE_FILL, depthClampEnable-VK_FALSE, rasterizerDiscardEnable-VK_FALSE, depthBiasEnable-VK_FALSE, lineWidth-1.0.
		/// </summary>
		/// <param name="polygonMode			"> : let vulkan turn triangles into points or lines.</param>
		/// <param name="cullMode			"> : determines the direction that used for cull triangles.</param>
		/// <param name="frontFace			"> : specifies the front direction in window space.</param>
		/// <param name="depthClampEnable		"> : if enabled, then will causes fragments those would have been clipped by near or far planes to instead be projected onto those planes.</param>
		/// <param name="rasterizerDiscardEnable	"> : if enabled, then will turn off the rasterization state and no fragments be produced.</param>
		/// <param name="lineWidth			"> : specifies the width of lines that rasterized, in pixels.</param>
		/// <param name="depthBiasEnable		"> : if enabled, the rasterization will compute a bias value with next three parameters to add to the depth value.</param>
		/// <param name="depthBiasSlopeFactor		"> : this parameters will scaled by a computed slope which represent the maximum depth slope of a triangle.</param>
		/// <param name="depthBiasConstantFactor	"> : this parameters will scaled by a constant value depended on the data type of depth buffer.</param>
		/// <param name="depthBiasClamp		"> : this parameters will limit the final computed bias.If the clamp value is positive, then it will be upper-bound of the bias.Otherwise, it will be lower-bound.</param>
		inline VkPipelineRasterizationStateCreateInfo aboutVkPipelineRasterizationStateCreateInfo() {
			VkPipelineRasterizationStateCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.polygonMode = VK_POLYGON_MODE_FILL;
			createInfo.depthClampEnable = VK_FALSE;
			createInfo.rasterizerDiscardEnable = VK_FALSE;
			createInfo.depthBiasEnable = VK_FALSE;
			createInfo.lineWidth = 1.0f;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY:rasterizationSamples.Optional:sampleShadingEnable, minSampleShading, pSampleMask, alphaToCoverageEnable, alphaToOneEnable.////
		/// If pipeline is being created with fragment output interface state, multisample state must be defined.
		/// </summary>
		/// <param name="rasterizationSamples		"> : specifies the number of samples per pixel.</param>
		/// <param name="sampleShadingEnable		"> : enable the function of sample shading.</param>
		/// <param name="minSampleShading		"> : if enable the sample shaing, the field specifies the minimum fraction of samples in the pixel will be shaded.</param>
		/// <param name="pSampleMask			"> : Using sample mask to update a subset of samples in the framebuffer.Every bit in the mask is corresponding to a sample point.</param>
		/// <param name="alphaToCoverageEnable	"> : controls whether a temporary coverage value is generated based on the alpha component of the fragment's first color output.</param>
		/// <param name="alphaToOneEnable		"> : controls whether the alpha component of the fragment's first color output is replaced with one.</param>
		inline VkPipelineMultisampleStateCreateInfo aboutVkPipelineMultisampleStateCreateInfo() {
			VkPipelineMultisampleStateCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;

			createInfo.sampleShadingEnable = VK_FALSE;
			createInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY:depthTestEnable, depthWriteEnable, depthCompareOp.
		/// OPTIONAL: depthBoundsTestEnable, minDepthBounds, maxDepthBounds, stencilTestEnable, front, back.////
		/// Because the depth and stencil test is optional, so i assume that you want to enable the depth test and specify the essential fields when using depth test.
		/// Stencil test can be different for front and back facing primitives.
		/// </summary>
		/// <param name="depthTestEnable		"> : enable the depth test.</param>
		/// <param name="depthWriteEnable		"> : enable the depth write operation.When enable the depth test, the resulting depth may be written to the depth buffer.</param>
		/// <param name="depthCompareOp		"> : specifies the compare operation of the depth test.</param>
		/// <param name="depthBoundsTestEnable	"> : enalbe the depth bounds test.By checking if the depth value falls into a specific range to determine if the fragment passes the bounds test.</param> 
		/// <param name="minDepthBounds		"> : specifies the minimum depth bounds.</param>
		/// <param name="maxDepthBounds		"> : specifies the maximum depth bounds.</param>
		/// <param name="stencilTestEnable		"> : enable the stencil test.</param>
		/// <param name="front				"> : stencil test for front-facing primitives.</param>
		/// <param name="back				"> : stencil test for back-facing primitives.</param>
		inline VkPipelineDepthStencilStateCreateInfo aboutVkPipelineDepthStencilStateCreateInfo() {
			VkPipelineDepthStencilStateCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;

			createInfo.depthBoundsTestEnable	= VK_FALSE;
			createInfo.stencilTestEnable		= VK_FALSE;

			return createInfo;
		}

		/// <summary>
		/// SPECIFY:failOp, passOp, depthFailOp, compareOp, compareMask, writeMask, reference.
		/// </summary>
		/// <param name="depthFailOp	"> : specifies the operation performed when the depth test fail.</param>
		/// <param name="failOp		"> : (depth test pass) specifies the operation performed when the stencil test fail.</param>
		/// <param name="passOp	"> : (depth test pass) specifies the operation performed when the stencil test pass.</param>
		/// <param name="compareOp	"> : specifies the compare operation between the reference value and the current content in the stencil buffer.</param>
		/// <param name="compareMask	"> : specifies the bit mask to select the bits in stencil values used for comparing.</param>
		/// <param name="writeMask	"> : specifies the bit mask to selecct the bits in stencil value used for updating the stencil buffer.</param>
		/// <param name="reference	"> : specifies the reference value to participate the comparision.</param>
		inline VkStencilOpState aboutVkStencilOpState() {
			VkStencilOpState stencilOp{};
			return stencilOp;
		}

		/// <summary>
		///	SPECIFY:blendEnable, srcColorBlendFactor, dstColorBlendFactor, colorBlendOp, srcAlphaBlendFactor, dstAlphaBlendFactor, alphaBlendOp, colorWriteMask.////
		/// Blending will enabled only for float formats.Also, you can use dual-source blending by specifing the factors to ONT_MINUS_SRC1_{COLOR/ALPHA}.
		/// </summary>
		/// <param name="blendEnable		"> : enable the blending to the corresponding color attachment.</param>
		/// <param name="srcColorBlendFactor	"> : specifies the coefficent that is multiplied to the source color.</param>
		/// <param name="dstColorBlendFactor	"> : specifies the coefficent that is multiplied to the destination color.</param>
		/// <param name="colorBlendOp		"> : the operation used for combine the results.</param>
		/// <param name="srcAlphaBlendFactor	"> : specifies the coefficent that is multiplied to the source's alpha channel.</param>
		/// <param name="dstAlphaBlendFactor	"> : specifies the coefficent that is multiplied to the destination's alpha channel.</param>
		/// <param name="alphaBlendOp		"> : the operation used for combine the results of alpha channels.</param>
		/// <param name="colorWriteMask	"> : selects the channels that will be written.</param>
		inline VkPipelineColorBlendAttachmentState aboutVkPipelineColorBlendAttachmentState() {
			VkPipelineColorBlendAttachmentState blendState{};
			blendState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			return blendState;
		}

		/// <summary>
		/// SPECIFY:logicOpEnable, logicOp, attachmentCount, pAttachments, blendConstant.////
		/// When enable the logic operation, all blending of attachments is treated as it were disabled.
		/// </summary>
		/// <param name="logicOpEnable		"> : enable the logic operation which is only applied for integer and normalized integer framebuffers.</param>
		/// <param name="logicOp		"> : specifies the logic operation.</param>
		/// <param name="attachmentCount	"> : the  number of color attachments.</param>
		/// <param name="pAttachments		"> : a pointer to an array of VkPipelineColorBlendAttachmentState structures.</param>
		/// <param name="blendConstant		"> : a pointer to an array of four float values which represent four components of RGBA.they are blend constants that are used in blending.</param>  
		inline VkPipelineColorBlendStateCreateInfo aboutVkPipelineColorBlendStateCreateInfo() {
			VkPipelineColorBlendStateCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY:dynamicStateCount, pDynamicStates.////
		/// Vulkan has the ability to mark parts of the graphics pipleine as dynamic to change the states in a high frequency.
		/// </summary>
		/// <param name="dynamicStateCount	"> : the number of statess that you want to set dynamic.</param>
		/// <param name="pDynamicStates	"> : specifies which states will use the values from the dynamic state commands.</param>
		inline VkPipelineDynamicStateCreateInfo aboutVkPipelineDynamicStateCreateInfo() {
			VkPipelineDynamicStateCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY:depth, stencil.
		/// </summary>
		/// <param name="depth	"> : clear value(float) for depth component.</param>
		/// <param name="stencil	"> : clear value(uint32_t) for stencil component.</param>
		inline VkClearDepthStencilValue aboutVkClearDepthStencilValue() {
			VkClearDepthStencilValue clearValue{};
			return clearValue;
		}

		/// <summary>
		/// SPECIFY:color, depthStencil.
		/// </summary>
		/// <param name="color		"> : clear value for color attachment.</param>
		/// <param name="depthStencil	"> : clear value for depth,stencil or depth-stencil attachment.</param>
		inline VkClearValue aboutVkClearValue() {
			VkClearValue clearValue{};
			return clearValue;
		}

		/// <summary>
		/// SPECIFY:renderPass, framebuffer, renderArea, clearValueCount, pClearValues.
		/// </summary>
		/// <param name="renderPass		"> : specifies the renderpass that is begun.</param>
		/// <param name="frameBuffer		"> : the framebuffer that the renderpass render into.</param>
		/// <param name="renderArea		"> : you can specify a small render area using offset and extent.</param>
		/// <param name="clearValueCount	"> : every attachment that have a CLEAR load operation needs a clear value, the number of clear values must be greater than the index of the last attachment that have a CLEAR operation.</param>
		/// <param name="pClearValues		"> : specifies the corresponding clear values.</param>
		inline VkRenderPassBeginInfo aboutVkRenderPassBeginInfo() {
			VkRenderPassBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			beginInfo.pNext = nullptr;
			return beginInfo;
		}

		/// <summary>
		/// SPECIFY:vertexCount, instanceCount, firstVertex, firstInstance.////
		/// Only used for vkCmdDrawIndirect, the structure need to be stored in a buffer.
		/// </summary>
		/// <param name="vertexCount	"> : the number of vertices processed by the draw command.</param>
		/// <param name="instanceCount	"> : used for instancing.Specifies the number of instances processed by the draw command.</param>
		/// <param name="firstVertex	"> : the first index of vertex fetched by the draw command.</param>
		/// <param name="fristInstance	"> : the first index of instance fetched by the draw command.</param>
		inline VkDrawIndirectCommand aboutVkDrawIndirectCommand() {
			VkDrawIndirectCommand command{};
			return command;
		}

		/// <summary>
		/// SPECIFY:indexCount, instanceCount, firstIndex, vertexOffset, firstInstance.////
		/// Only used for vkCmdDrawIndexedIndirect, the structure need to be stored in a buffer.
		/// </summary>
		/// <param name="indexCount	"> : the number of indices processed by the draw command.</param>
		/// <param name="instanceCount	"> : the number of instances processed by the draw command.</param>
		/// <param name="firstIndex	"> : specifies where to start fetching indices from the index buffer.</param>
		/// <param name="vertexOffset	"> : the additional value to be added to the result of the vertex index computed by the firstIndex and offset.</param>
		/// <param name="firstInstance	"> : specifies the first instances.</param>
		inline VkDrawIndexedIndirectCommand aboutVkDrawIndexedIndirectCommand() {
			VkDrawIndexedIndirectCommand command{};
			return command;
		}

		/// <summary>
		/// SPECIFY:flags, queueFamilyIndex.
		/// </summary>
		/// <param name="flags			"> : determines the behavior of the pool and the command buffers that allocated from it.</param>
		/// <param name="queueFamilyIndex	"> : speicfies the queue family to which command buffers allocated from this pool.</param>
		inline VkCommandPoolCreateInfo aboutVkCommandPoolCreateInfo() {
			VkCommandPoolCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			createInfo.pNext = nullptr;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY:commandPool, level, commandBufferCount.////
		/// Use for vkAllocateCommandBuffers.
		/// </summary>
		/// <param name="commandPool		"> : indicates that which command pool used for command buffer allocation.</param>
		/// <param name="level			"> : specifies the level of the command buffer.It can be PRIMARY or SECONDARY.</param>
		/// <param name="commandBufferCount	"> : the number of the command buffers we want to allocate from the pool.</param>
		inline VkCommandBufferAllocateInfo aboutVkCommandBufferAllocateInfo() {
			VkCommandBufferAllocateInfo allocateInfo{};
			allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocateInfo.pNext = nullptr;
			return allocateInfo;
		}

		//TODO:pInheritanceInfo.

		/// <summary>
		/// SPEICFY:flags, pInheritanceInfo.
		/// </summary>
		/// <param name="flags"> : specifies the usage of the command buffer.It can be ONE_TIME flag, which means that command buffer will be recorded, executed only once, 
		/// and RENDER_PASS flag, which is valid only for secondary command buffer,and SIMULTANEOUS flag, which means the command buffer might be executed or pending execution more than once.
		/// </param>
		inline VkCommandBufferBeginInfo aboutVkCommandBufferBeginInfo() {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.pNext = nullptr;
			beginInfo.flags = 0;
			return beginInfo;
		}

		/// <summary>
		/// SPECIFY:flags.////
		/// A fence object only cause the host to wait on.A primary use case for fences is to prevent the host from overwriting data that might be in use by the device.
		/// </summary>
		/// <param name="flags"> : VK_FENCE_CREATE_SIGNALED_BIT,means the initial state of the fence is in signaled.</param>
		inline VkFenceCreateInfo aboutVkFenceCreateInfo() {
			VkFenceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			createInfo.pNext = nullptr;
			return createInfo;
		}

		/// <summary>
		/// OPTIONAL: flags.
		/// </summary>
		/// <param name="flags"> : In vulkan1.3, the only value is VK_EVENT_CREATE_DEVICE_ONLY_BIT.</param>
		inline VkEventCreateInfo aboutVkEventCreateInfo() {
			VkEventCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			return createInfo;
		}

		/// <summary>
		/// NO SPECIFY FOR NOW.Semaphores are usually used in structures which contain some parameters such as pWaitSemaphores and pSignalSemaphores.
		/// Semaphores can synchronize operations of access to resources in different queues.Such operations as require a image from swapchain then call drawing commnand on the required image.
		/// </summary>
		inline VkSemaphoreCreateInfo aboutVkSemaphoreCreateInfo() {
			VkSemaphoreCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			return createInfo;
		}

		/// <summary>
		/// SPECIFY:waitSemaphoreCount, pWaitSemaphores, pWaitDstStageMask, commandBufferCount, pCommandBuffers, signalSemaphoreCount, pSignalSemaphores.////
		///	You can submit multiple command buffers in a single submission.And all command buffers can only be executed when the condition is met. 
		/// </summary>
		/// <param name="waitSemaphoreCount		"> : the number of sepachores which to wait before execute the commmand buffers.</param>
		/// <param name="pWaitSemaphores		"> : a pointer to an array of fVkSemaphore handles.</param>
		/// <param name="pWaitDstStageMask		"> : a pointer to an array of pipeline stages at which each corresponding semaphore wait will occur.</param>
		/// <param name="commandBufferCount		"> : the number of command buffers to execute.</param>
		/// <param name="pCommandBuffers		"> : a pointer to an array of command buffers.</param>
		/// <param name="signalSemaphoreCount	"> : the number of semaphores that will be signaled after command buffers have completed execution.</param>
		/// <param name="pSignalSemaphores		"> : a pointer to an array of VkSemaphore handles.</param>
		inline VkSubmitInfo aboutVkSubmitInfo() {
			VkSubmitInfo submit{};
			submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submit.pNext = nullptr;
			return submit;
		}

		/// <summary>
		/// SPECIFY:waitSemaphoreCount, pWaitSemaphores, swapchainCount, pSwapchains, pImageIndices, pResult.////
		/// A single call to vkQueuePresentKHR can present multiple images which in multiple swapchains.And the images presented by the command are referenced by indices instead of image handles. 
		/// </summary>
		/// <param name="waitSemaphoreCount	"> : the number of semaphores which to wait before present.</param>
		/// <param name="pWaitSemaphores	"> : a pointer to an array of VkSemaphore handles.</param>
		/// <param name="swapchainCount	"> : the number of images need to presented.</param>
		/// <param name="pSwapchains		"> : the swapchains which contain the images.</param>
		/// <param name="pImageIndices		"> : the indices of the images in the swapchains.</param>
		/// <param name="pResult		"> : each presentaion has its own result code.</param>
		inline VkPresentInfoKHR aboutVkPresentInfoKHR() {
			VkPresentInfoKHR present{};
			present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			present.pNext = nullptr;
			return present;
		}

		/// <summary>
		/// SPECIFY:magFilter, minFilter, mipmapMode, addressModeU, addressModeV, addressModeW, borderColor.
		/// OPTIONAL:mipLodBias, anisotropyEnable, maxAnisotropy, compareEnable, compareOp, minLod, maxLod, unnormalizedCoordinates.
		/// DEFAULT:anisotropyEnable - VK_FALSE, compareEnable - VK_FALSE, unnormalizedCoordinates - VK_FALSE, minLod - 0.f, maxLod - VK_LOD_CLAMP_NONE.
		/// </summary>
		/// <param name="magFilter			"> : specifies the filtering mode when the image is magnified.</param>
		/// <param name="minFilter			"> : specifies the filtering mode when the image is minified.</param>
		/// <param name="mipmapMode			"> : specifies the mipmaps usage when sampled.</param>
		/// <param name="addressMode{U,V,W}		"> : specifies the transform that is applied to texture coordinates when sample outside.</param>
		/// <param name="borderColor			"> : For the VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER filter mode, when texels are sampled from the border, the border color is specified in this field.</param> 
		/// <param name="mipLodBias			"> : specifies a floating-point bias that is added to the lod before mipmap selection.</param>
		/// <param name="anisotropyEnable		"> : detemines that enable the anisotropic filtering or not.</param>
		/// <param name="maxAnisotropy		"> : a value between 1 and the maximum value supported by the device.Specifies the maximum of anisotropy.</param>
		/// <param name="compareEnable		"> : determines that enable the compare operation or not when the sampler used with a depth image or something similar.</param>
		/// <param name="compareOp			"> : specifies the compare operation.</param>
		/// <param name="minLod			"> : specifies the minimum lod that can be sampled by the sampler.</param>
		/// <param name="maxLod			"> : specifies the maximum lod that can be sampled by the sampler.</param>
		/// <param name="unnormalizedCoordinates	"> : determines that the coordinates used to sample the image are in units of texels or normalized.
		///		If is VK_TRUE, minFilter nad magFilter must bt the same, mipmapMode must be VK_SAMPLER_MIPMAP_MODE_NEAREST, and anisotropyEnable and compareEnable must be VK_FALSE.</param>
		inline VkSamplerCreateInfo aboutVkSamplerCreateInfo() {
			VkSamplerCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			//default
			createInfo.anisotropyEnable			= VK_FALSE;
			createInfo.compareEnable			= VK_FALSE;
			createInfo.unnormalizedCoordinates	= VK_FALSE;
			createInfo.minLod = 0.f;
			createInfo.maxLod = VK_LOD_CLAMP_NONE;

			return createInfo;
		}

		//TODO:srcQueueFamilyIndex, dstQueueFamilyIndex.

		/// <summary>
		/// SPECIFY:oldLayout, newLayout, image, subresourceRange, srcAccessMask, dstAccessMask. OPTIONAL:srcQueueFamilyIndex, dstQueueFamilyIndex.
		/// DEFAULT:srcQueueFamilyIndex - VK_QUEUE_FAMILY_IGNORED, dstQueueFamilyIndex - VK_QUEUE_FAMILY_IGNORED.////
		/// If there is no transfer of ownership across queues, both srcQueueFamilyIndex and dstQueueFamilyInde are set to VK_QUEUE_FAMILY_IGNORED.
		/// </summary>
		/// <param name="oldLayout		"> : specifies the old layout of the image before execute barrier command.</param>
		/// <param name="newLayout		"> : specifies the new layout of the image after execute barrier command.</param>
		/// <param name="image			"> : specifies the image affected by barrier command.</param>
		/// <param name="subresourceRange	"> : specifies the subresource of the image affected by barrier command.</param>
		/// <param name="srcAccessMask	"> : specifies how the image was last accessed.</param>
		/// <param name="dstAccessMask	"> : specifies how the image was next to be accessed</param>
		inline VkImageMemoryBarrier aboutVkImageMemoryBarrier() {
			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.pNext = nullptr;

			//default
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			return barrier;
		}
#endif // VULKAN_INFO 0

#if VULKAN_INFO > 0
		inline VkBindBufferMemoryInfo aboutVkBindBufferMemoryInfo();

		inline VkBindImageMemoryInfo abuoutVkBindImageMemoryInfo();

		/// <summary>
		/// Usually using this structure to require device's features. In Vulkan1.1, you must specify the sType field of this so that you can
		/// call vkGetPhysicalDeviceFeatures2 correctly;
		/// You can assign another supportive structure to pNext to require extension features. 
		/// </summary>
		inline VkPhysicalDeviceFeatures2 aboutVkPhisicalDeviceFeatures2() {
			VkPhysicalDeviceFeatures2 features2{};
			features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
			return features2;
		}

		/// <summary>
		/// This structure provided by VK_EXT_robustness2. You require extension features by assign this structure to VkPhysicalDeviceFeaatures2.pNext when you 
		/// call VkGetPhysicalDeviceFeatures2.
		/// If you want to enable some extension features on device, assign this to VkDeviceCreateInfo.pNext when you create it.
		/// </summary>
		inline VkPhysicalDeviceRobustness2FeaturesEXT abouotVkPhysicalDeviceRobustness2FeaturesEXT() {
			VkPhysicalDeviceRobustness2FeaturesEXT featuresEXT{};
			featuresEXT.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT;
			return featuresEXT;
		}
#endif // VULKAN_INFO 1

#if VULKAN_INFO > 1
		inline VkBufferCopy2KHR aboutVkBufferCopy2KHR();
#endif

	}
}
