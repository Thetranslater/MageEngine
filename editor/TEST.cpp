#include<core/math/matrix4x4.h>

#include<engine_core/render_engine/window_system.h>
#include<engine_core/render_engine/renderer/vulkanRHI.h>
#include<engine_core/render_engine/renderer/vulkanInfo.h>
#include<engine_core/render_engine/renderer/vulkanHelper.h>

#include<core/macro.h>

#include<asset/resource_manager/resource_manager.h>

#include<glm-master/glm/glm.hpp>

#define mgvk  Mage::VulkanInfo
#define STR(s) #s

std::shared_ptr<Mage::WindowSystem> window = std::make_shared<Mage::WindowSystem>();
std::shared_ptr<Mage::VulkanRHI> rhi = std::make_shared<Mage::VulkanRHI>();

std::vector<VkBuffer> buffers;
std::vector<VkDeviceMemory> memories;

struct Image {
	VkImage data;
	VkImageView view;
	VkDeviceMemory memory;
	VkSampler sampler;
};

struct PerFrame {
	glm::mat4 view;
	glm::mat4 perspective;
};

struct UBO {
	glm::mat4 model[60];
};

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 tangent;
	glm::vec2 texcoord_0;

	static std::array<VkVertexInputBindingDescription, 4> getVkVertexInputBindingDescription() {
		std::array<VkVertexInputBindingDescription, 4> descriptions;
		for (int i{ 0 }; i < 4; ++i) {
			descriptions[i] = mgvk::aboutVkVertexInputBindingDescription();
			descriptions[i].binding = i;
			descriptions[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}
		descriptions[0].stride = sizeof(glm::vec3);
		descriptions[1].stride = sizeof(glm::vec3);
		descriptions[2].stride = sizeof(glm::vec4);
		descriptions[3].stride = sizeof(glm::vec2);
		return descriptions;
	}

	static std::array<VkVertexInputAttributeDescription, 4> getVkVertexInputAttributeDescription() {
		std::array < VkVertexInputAttributeDescription, 4> attributes;
		for (int i{ 0 }; i < 4; ++i) {
			attributes[i] = mgvk::aboutVkVertexInputAttributeDescription();
			attributes[i].binding = i;
			attributes[i].location = i;
			attributes[i].offset = 0;
		}
		attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributes[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributes[3].format = VK_FORMAT_R32G32_SFLOAT;
		return attributes;
	}
};

struct Mesh {
	std::array<std::tuple<uint32_t, uint32_t>, 4> attributes_des;

	uint32_t index_count{ 0 };
	uint32_t index_offset;

	glm::mat4 mesh_matrix = glm::mat4(1.f);
	uint32_t material_index;
};

struct Material {
	VkImageView albedo;
	VkSampler albedo_sampler;
	VkImageView normal;
	VkSampler normal_sampler;
	VkImageView metallic_roughness;
	VkSampler metallic_roughness_sampler;

	VkDescriptorSet set;
};

struct RenderModel {
	VkBuffer super_block;
	VkDeviceMemory super_memory;
	std::vector<Image> images;

	std::vector<Material> materials;
	std::vector<Mesh> meshes;
};

class Pipeline {
public:
	void initialize(std::shared_ptr<Mage::VulkanRHI>);

	void draw(RenderModel&);
private:
	void setupDescriptorSetLayouts();
	void setupDescriptorSets();
	void setupPieplines();
	void setupRenderpass();
	void setupFrameBuffers();
public:
	std::shared_ptr<Mage::VulkanRHI> rhi;
	VkPipeline pipeline;
	VkPipelineLayout pipeline_layout;
	std::vector<VkDescriptorSetLayout> descriptorset_layouts;
	std::vector<VkDescriptorSet> descriptorsets;
	std::vector<VkFramebuffer> framebuffers;
	VkRenderPass render_pass;
};

#pragma region PipelineDefine
void Pipeline::initialize(std::shared_ptr<Mage::VulkanRHI> vk_rhi) { 
	rhi = vk_rhi; 
	setupDescriptorSetLayouts();
	setupDescriptorSets();
	setupRenderpass();
	setupFrameBuffers();
	setupPieplines();
}

void Pipeline::setupDescriptorSetLayouts() {
	descriptorset_layouts.resize(2);
	//uniform buffer object
	{
		VkDescriptorSetLayoutBinding ubo_binding = mgvk::aboutVkDescriptorSetLayoutBinding();
		ubo_binding.binding = 1;
		ubo_binding.descriptorCount = 1;
		ubo_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		ubo_binding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutBinding perframe_binding = mgvk::aboutVkDescriptorSetLayoutBinding();
		perframe_binding.binding = 0;
		perframe_binding.descriptorCount = 1;
		perframe_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		perframe_binding.pImmutableSamplers = nullptr;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { perframe_binding, ubo_binding };

		VkDescriptorSetLayoutCreateInfo ubo_layout = mgvk::aboutVkDescriptorSetLayoutCreateInfo();
		ubo_layout.bindingCount = 2;
		ubo_layout.pBindings = bindings.data();

		ASSERT_RESULT(vkCreateDescriptorSetLayout(rhi->getDevice(), &ubo_layout, nullptr, &descriptorset_layouts[0]));
	}

	{
		VkDescriptorSetLayoutBinding texturebinding[3] = {};
		texturebinding[0] = mgvk::aboutVkDescriptorSetLayoutBinding();
		texturebinding[0].binding = 0;
		texturebinding[0].descriptorCount = 1;
		texturebinding[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		texturebinding[0].pImmutableSamplers = nullptr;
		texturebinding[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		texturebinding[1] = texturebinding[0];
		texturebinding[1].binding = 1;
		texturebinding[2] = texturebinding[0];
		texturebinding[2].binding = 2;

		VkDescriptorSetLayoutCreateInfo texture_layout = mgvk::aboutVkDescriptorSetLayoutCreateInfo();
		texture_layout.bindingCount = 3;
		texture_layout.pBindings = texturebinding;
		ASSERT_RESULT(vkCreateDescriptorSetLayout(rhi->getDevice(), &texture_layout, nullptr, &descriptorset_layouts[1]));
	}
}

void Pipeline::setupDescriptorSets() {
	descriptorsets.resize(rhi->getSwapchainSize());
	std::vector<VkDescriptorSetLayout> layouts(rhi->getSwapchainSize(), descriptorset_layouts[0]);
	VkDescriptorSetAllocateInfo ubo_alloc = mgvk::aboutVkDescriptorSetAllocateInfo();
	ubo_alloc.descriptorPool = rhi->getDescriptorPool();
	ubo_alloc.descriptorSetCount = rhi->getSwapchainSize();
	ubo_alloc.pSetLayouts = layouts.data();

	ASSERT_RESULT(vkAllocateDescriptorSets(rhi->getDevice(), &ubo_alloc, &descriptorsets[0]));

	for (int i{ 0 }; i < rhi->getSwapchainSize(); ++i) {
		VkDescriptorBufferInfo bufferinfo[2];
		bufferinfo[0] = mgvk::aboutVkDescriptorBufferInfo();
		bufferinfo[0].buffer = buffers[i];
		bufferinfo[0].offset = 0;
		bufferinfo[0].range = sizeof(PerFrame);

		bufferinfo[1] = bufferinfo[0];
		bufferinfo[1].range = sizeof(UBO);

		VkWriteDescriptorSet write[2];
		write[0] = mgvk::aboutVkWriteDescriptorSet();
		write[0].descriptorCount = 1;
		write[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		write[0].dstArrayElement = 0;
		write[0].dstBinding = 0;
		write[0].dstSet = descriptorsets[i];
		write[0].pBufferInfo = &bufferinfo[0];

		write[1] = write[0];
		write[1].dstBinding = 1;
		write[1].pBufferInfo = &bufferinfo[1];

		vkUpdateDescriptorSets(rhi->getDevice(), 2, write, 0, nullptr);
	}
}

void Pipeline::setupRenderpass() {
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = rhi->getSwapchainSurfaceFormat().format;
	//目前不做任何重采样，所以我们保持1sample
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	//loadop&storeop 应用在color和depth data，而stencilop应用在stencil data
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	//现在我们的renderpass多了一个depth attachment
	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = Mage::VulkanHelper::findDepthFormat(rhi.get(), 
		{ VK_FORMAT_D32_SFLOAT,VK_FORMAT_D32_SFLOAT_S8_UINT,VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT); //应和depth image的format一致
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;//attachment array的下标
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//subpasses
	VkSubpassDescription subpass{};
	//vulkan将来可能还支持其他类型的subpass，所以我们需要显式指定这是一个graphics subpass
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

	subpass.colorAttachmentCount = 1;
	//直接引用到fragment shader中的outColor……HOW?
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;//一个subpass只能使用一个depth(+stencil)attachment


	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = 0;

	//
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment,depthAttachment };
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(rhi->getDevice(), &renderPassInfo, nullptr, &render_pass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create  render pass");
	}
}

void Pipeline::setupFrameBuffers() {
	framebuffers.resize(rhi->getSwapchainSize());
	for (int i{ 0 }; i < framebuffers.size(); ++i) {
		VkImageView attachments[2] = { rhi->getSwapchainImageView(i),rhi->getDepthImageView()};
		VkFramebufferCreateInfo framebuffer_info = mgvk::aboutVkFramebufferCreateInfo();
		framebuffer_info.renderPass = render_pass;
		framebuffer_info.attachmentCount = 2;
		framebuffer_info.pAttachments = attachments;
		framebuffer_info.width = rhi->getSwapchainExtent().width;
		framebuffer_info.height = rhi->getSwapchainExtent().height;
		framebuffer_info.layers = 1;

		ASSERT_RESULT(vkCreateFramebuffer(rhi->getDevice(), &framebuffer_info, nullptr, &framebuffers[i]));
	}
}

void Pipeline::setupPieplines() {
	VkShaderModule vertShaderModule = Mage::VulkanHelper::shaderModuleCreationHelper(rhi->getDevice(),"E:\\Mage\\engine\\shaders\\test_vert.spv");
	VkShaderModule fragShaderModule = Mage::VulkanHelper::shaderModuleCreationHelper(rhi->getDevice(), "E:\\Mage\\engine\\shaders\\test_frag.spv");

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//stage告诉vulkan该shader什么时候被使用
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";
	//可选成员pSpecializationInfo,它允许你指定shader constants的值

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo,fragShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

	auto bindingDescription = Vertex::getVkVertexInputBindingDescription();
	auto attributeDescriptions = Vertex::getVkVertexInputAttributeDescription();

	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 4;
	//pVertexBinding……和pVertexAttribute……指向一个结构数组，描述vertex data的前述细节
	vertexInputInfo.pVertexBindingDescriptions = bindingDescription.data(); //optional
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); //optional

	//VkPipelineInputAssemblyStateCreateInfo告诉我们怎样的图形需要绘制以及是否启动图元重启
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	//VkViewpot告诉我们framebuffer中需要输出渲染的图像大小
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = rhi->getSwapchainExtent().width;
	viewport.height = rhi->getSwapchainExtent().height;
	//minDepth和maxDepth指出了为framebuffer使用的depth value的范围
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0,0 };
	scissor.extent = rhi->getSwapchainExtent();

	//上面两个struct用来创建viewportcreateinfo，有的GPU feature允许创建多个viewport，所以它们以数组指针形式传递
	VkPipelineViewportStateCreateInfo viewportState{};;
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;

	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	//detemine the face culling type.我们可以cull掉front face或back face或both。face front设置为顺时针或逆时针
	rasterizer.cullMode = VK_CULL_MODE_NONE;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f;
	rasterizer.depthBiasClamp = 0.0f;
	rasterizer.depthBiasSlopeFactor = 0.0f;

	//one way to perform anti-aliasing
	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	//VkPipelineColorBlendStateCreateInfo包含全局color blending settings
	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 2; //descriptorLayout的数量
	pipelineLayoutInfo.pSetLayouts = descriptorset_layouts.data(); //单独的descriptorLayout，包含一个数组
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(rhi->getDevice(), &pipelineLayoutInfo, nullptr, &pipeline_layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
	//为了启用深度测试，我们还需再pipeline中配置深度测试
	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE; //新fragment数据是否需要和depth image中原数据对比，测试是否需要丢弃
	depthStencil.depthWriteEnable = VK_TRUE; //通过了测试的fragment是否需要写入depth image

	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;//越近处的物体，深度越小

	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.maxDepthBounds = 1.0f;
	depthStencil.minDepthBounds = 0.0f;

	//用来配置stencil buffer operation，如果使用，你需要保证depth/stencil image含有stencil component
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; //optional
	depthStencil.back = {}; //optional

	VkGraphicsPipelineCreateInfo pipelineInfo = mgvk::aboutVkGraphicsPipelineCreateInfo();
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	//存者shaderModule
	pipelineInfo.pStages = shaderStages;

	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr;

	pipelineInfo.layout = pipeline_layout;

	pipelineInfo.renderPass = render_pass;
	pipelineInfo.subpass = 0;

	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(rhi->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}
	//局部变量的销毁
	vkDestroyShaderModule(rhi->getDevice(), fragShaderModule, nullptr);
	vkDestroyShaderModule(rhi->getDevice(), vertShaderModule, nullptr);
}

void Pipeline::draw(RenderModel& model) {
	void* begin;
	vkMapMemory(rhi->getDevice(), memories[rhi->getCurrentFrameIndex()], 0, 1024*1024*20, 0, &begin);

	uint32_t perframe_offset{ 0 };
	(reinterpret_cast<PerFrame*>(reinterpret_cast<uint8_t*>(begin) + perframe_offset))->view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	(reinterpret_cast<PerFrame*>(reinterpret_cast<uint8_t*>(begin) + perframe_offset))->perspective = glm::perspective(glm::radians(4.5f), rhi->getSwapchainExtent().width / (float)rhi->getSwapchainExtent().height, 0.5f, 10.0f);
	(reinterpret_cast<PerFrame*>(reinterpret_cast<uint8_t*>(begin) + perframe_offset))->perspective[1][1] *= -1;

	rhi->waitForFences();

	rhi->prepareContext();

	rhi->prepareVulkanRHIBeforeRender();

	VkRenderPassBeginInfo renderpassInfo = mgvk::aboutVkRenderPassBeginInfo();
	std::array<VkClearValue, 2> clearValues{};//clearValues内部顺序应该和你attachments的顺序一致
	clearValues[0].color = { {0.0f,0.0f,0.0f,1.0f} };
	clearValues[1].depthStencil = { 1.0f,0 };//depth range in vulkan is 0-1, which 1 means far plane
	renderpassInfo.clearValueCount = 2;
	renderpassInfo.pClearValues = clearValues.data();
	renderpassInfo.framebuffer = framebuffers[rhi->getSwapchainIndex()];
	renderpassInfo.renderPass = render_pass;
	renderpassInfo.renderArea.offset = { 0,0 };
	renderpassInfo.renderArea.extent = rhi->getSwapchainExtent();


	vkCmdBeginRenderPass(rhi->getCurrentCommandBuffer(), &renderpassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(rhi->getCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

	uint32_t perdrawcall_off{ perframe_offset };
	perdrawcall_off += sizeof(PerFrame);
	perdrawcall_off = Mage::Mathf::RoundUp(perdrawcall_off, rhi->getDeviceProperties().limits.minUniformBufferOffsetAlignment);

	for (auto& mesh : model.meshes) {
		//update uniform buffer
		(reinterpret_cast<UBO*>(reinterpret_cast<uint8_t*>(begin) + perdrawcall_off))->model[0] = mesh.mesh_matrix;

		vkCmdBindDescriptorSets(rhi->getCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 1, 1, &model.materials[mesh.material_index].set, 0, nullptr);

		VkBuffer buffers[4] = { model.super_block,model.super_block, model.super_block, model.super_block};
		VkDeviceSize offsets[4] = { std::get<0>(mesh.attributes_des[0]), 
			std::get<0>(mesh.attributes_des[1]),
			std::get<0>(mesh.attributes_des[2]),
			std::get<0>(mesh.attributes_des[3]) };
		vkCmdBindVertexBuffers(rhi->getCurrentCommandBuffer(), 0, 4, buffers, offsets);
		vkCmdBindIndexBuffer(rhi->getCurrentCommandBuffer(), model.super_block, mesh.index_offset, VK_INDEX_TYPE_UINT32);

		uint32_t temp[2] = {perframe_offset, perdrawcall_off};
		vkCmdBindDescriptorSets(rhi->getCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_layout, 0, 1, &descriptorsets[rhi->getCurrentFrameIndex()], 2, temp);

		vkCmdDrawIndexed(rhi->getCurrentCommandBuffer(), mesh.index_count, 1, 0, 0, 0);
		
		perdrawcall_off += sizeof(UBO);
		perdrawcall_off = Mage::Mathf::RoundUp(perdrawcall_off, rhi->getDeviceProperties().limits.minUniformBufferOffsetAlignment);
	}

	vkCmdEndRenderPass(rhi->getCurrentCommandBuffer());

	rhi->prepareVulkanRHIAfterRender();

	vkUnmapMemory(rhi->getDevice(), memories[rhi->getCurrentFrameIndex()]);
}
#pragma endregion

void _process_node(RenderModel& ret_model, const std::vector<Mage::Node>& meshes, uint32_t index, const glm::mat4& parent_mat) {
	auto& node = meshes[index];
	glm::mat4 curr_mat(1.f);
	if (!node.m_matrix.empty()) {
		curr_mat = glm::mat4(node.m_matrix[0], node.m_matrix[1], node.m_matrix[2], node.m_matrix[3],
			node.m_matrix[4], node.m_matrix[5], node.m_matrix[6], node.m_matrix[7],
			node.m_matrix[8], node.m_matrix[9], node.m_matrix[10], node.m_matrix[11],
			node.m_matrix[12], node.m_matrix[13], node.m_matrix[14], node.m_matrix[15]);
	}
	else if (!node.m_scale.empty() && !node.m_rotation.empty() && !node.m_translation.empty()) {
		Mage::Matrix4x4 m;
		m.SetTRS(node.m_translation, node.m_rotation, node.m_scale);
		curr_mat = glm::mat4(m);
	}
	curr_mat *= parent_mat;
	if (node.m_mesh != -1) {
		ret_model.meshes[node.m_mesh].mesh_matrix = curr_mat;
	}

	if (!node.m_children.empty()) {
		for (auto i : node.m_children) {
			_process_node(ret_model, meshes, i, curr_mat);
		}
	}
}

RenderModel loadModel(const std::string& file) {
	Mage::ResourceManager loader;
	Mage::Model model;
	loader.loadMageModel(file, &model, nullptr, nullptr, true);

	RenderModel ret_model{};

	Mage::VulkanHelper::bufferCreationHelper(rhi.get(),
		model.m_buffers.front().m_data.size(),
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		ret_model.super_block, ret_model.super_memory);

	Mage::VulkanHelper::moveDataFromVectorToBuffer(rhi.get(), 
		model.m_buffers.front().m_data.begin(), 
		model.m_buffers.front().m_data.end(), 
		ret_model.super_block);

	model.m_buffers.clear();
	VkDescriptorSetLayoutBinding bindings[3] = {};
	bindings[0] = mgvk::aboutVkDescriptorSetLayoutBinding();
	bindings[0].binding = 0;
	bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	bindings[0].descriptorCount = 1;
	bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	bindings[1] = bindings[0];
	bindings[1].binding = 1;
	bindings[2] = bindings[0];
	bindings[2].binding = 2;

	VkDescriptorSetLayoutCreateInfo layout_info = mgvk::aboutVkDescriptorSetLayoutCreateInfo();
	layout_info.bindingCount = 3;
	layout_info.pBindings = bindings;
	
	VkDescriptorSetLayout layout;
	ASSERT_RESULT(vkCreateDescriptorSetLayout(rhi->getDevice(), &layout_info, nullptr, &layout));	

	std::unordered_map<uint32_t, std::pair<VkImageView,VkSampler>> index_imageview_map;
	for (uint32_t i{ 0 }; i < model.m_materials.size(); ++i) {
		Material material_i;
		if (index_imageview_map.find(model.m_materials[i].m_pbr_metallic_roughness.m_base_color_texture.m_index) == index_imageview_map.end()) {
			Image albedo;
			int index = model.m_materials[i].m_pbr_metallic_roughness.m_base_color_texture.m_index;
			uint32_t size = model.m_textures[index].m_width * model.m_textures[index].m_height * 4;

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingMemory;
			Mage::VulkanHelper::bufferCreationHelper(rhi.get(), size,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				stagingBuffer, stagingMemory);
			void* p;
			vkMapMemory(rhi->getDevice(), stagingMemory, 0, size, 0, &p);
			memcpy(p, model.m_textures[index].m_data.data(), size);
			vkUnmapMemory(rhi->getDevice(), stagingMemory);

			Mage::VulkanHelper::imageCreationHelper(rhi.get(), model.m_textures[index].m_width, model.m_textures[index].m_height,
				VK_FORMAT_R8G8B8A8_SRGB, 1, VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				albedo.data, albedo.memory);

			Mage::VulkanHelper::transitionImageLayout(rhi.get(), albedo.data, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1);

			Mage::VulkanHelper::fromBufferToImageCopyHelper(rhi.get(), stagingBuffer, albedo.data, model.m_textures[index].m_width, model.m_textures[index].m_height);

			Mage::VulkanHelper::transitionImageLayout(rhi.get(), albedo.data, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1);
			
			VkImageViewCreateInfo al_v = mgvk::aboutVkImageViewCeateInfo();
			al_v.image = albedo.data;
			al_v.format = VK_FORMAT_R8G8B8A8_SRGB;
			al_v.components.a = VK_COMPONENT_SWIZZLE_A;
			al_v.components.r = VK_COMPONENT_SWIZZLE_R;
			al_v.components.g = VK_COMPONENT_SWIZZLE_G;
			al_v.components.b = VK_COMPONENT_SWIZZLE_B;
			al_v.viewType = VK_IMAGE_VIEW_TYPE_2D;
			al_v.subresourceRange = mgvk::aboutVkImageSubresourceRange();
			al_v.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			al_v.subresourceRange.baseArrayLayer = 0;
			al_v.subresourceRange.baseMipLevel = 0;
			al_v.subresourceRange.layerCount = 1;
			al_v.subresourceRange.levelCount = 1;
			ASSERT_RESULT(vkCreateImageView(rhi->getDevice(), &al_v, nullptr, &albedo.view));

			VkSamplerCreateInfo al_s = mgvk::aboutVkSamplerCreateInfo();
			al_s.magFilter = VK_FILTER_LINEAR;
			al_s.minFilter = VK_FILTER_LINEAR;
			al_s.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			al_s.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			al_s.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			al_s.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
			al_s.minLod = 0.f;
			al_s.maxLod = VK_LOD_CLAMP_NONE;
			ASSERT_RESULT(vkCreateSampler(rhi->getDevice(),&al_s,nullptr,&albedo.sampler))

			ret_model.images.push_back(albedo);
			index_imageview_map.insert(std::make_pair(index, std::make_pair(albedo.view, albedo.sampler)));

			vkDestroyBuffer(rhi->getDevice(), stagingBuffer, nullptr);
			vkFreeMemory(rhi->getDevice(), stagingMemory, nullptr);
		}
		material_i.albedo = index_imageview_map[model.m_materials[i].m_pbr_metallic_roughness.m_base_color_texture.m_index].first;
		material_i.albedo_sampler = index_imageview_map[model.m_materials[i].m_pbr_metallic_roughness.m_base_color_texture.m_index].second;
		if (index_imageview_map.find(model.m_materials[i].m_pbr_metallic_roughness.m_metallic_roughness_texture.m_index) == index_imageview_map.end()) {
			Image m_r;
			int index = model.m_materials[i].m_pbr_metallic_roughness.m_metallic_roughness_texture.m_index;
			uint32_t size = model.m_textures[index].m_width * model.m_textures[index].m_height * 4;

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingMemory;
			Mage::VulkanHelper::bufferCreationHelper(rhi.get(), size,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				stagingBuffer, stagingMemory);
			void* p;
			vkMapMemory(rhi->getDevice(), stagingMemory, 0, size, 0, &p);
			memcpy(p, model.m_textures[index].m_data.data(), size);
			vkUnmapMemory(rhi->getDevice(), stagingMemory);

			Mage::VulkanHelper::imageCreationHelper(rhi.get(), model.m_textures[index].m_width, model.m_textures[index].m_height,
				static_cast<VkFormat>(model.m_textures[index].m_format), 1, VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				m_r.data, m_r.memory);

			Mage::VulkanHelper::transitionImageLayout(rhi.get(), m_r.data, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1);

			Mage::VulkanHelper::fromBufferToImageCopyHelper(rhi.get(), stagingBuffer, m_r.data, model.m_textures[index].m_width, model.m_textures[index].m_height);

			Mage::VulkanHelper::transitionImageLayout(rhi.get(), m_r.data, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1);
			
			VkImageViewCreateInfo al_v = mgvk::aboutVkImageViewCeateInfo();
			al_v.image = m_r.data;
			al_v.format = static_cast<VkFormat>(model.m_textures[index].m_format);
			al_v.components.a = VK_COMPONENT_SWIZZLE_A;
			al_v.components.r = VK_COMPONENT_SWIZZLE_R;
			al_v.components.g = VK_COMPONENT_SWIZZLE_G;
			al_v.components.b = VK_COMPONENT_SWIZZLE_B;
			al_v.viewType = VK_IMAGE_VIEW_TYPE_2D;
			al_v.subresourceRange = mgvk::aboutVkImageSubresourceRange();
			al_v.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			al_v.subresourceRange.baseArrayLayer = 0;
			al_v.subresourceRange.baseMipLevel = 0;
			al_v.subresourceRange.layerCount = 1;
			al_v.subresourceRange.levelCount = 1;
			ASSERT_RESULT(vkCreateImageView(rhi->getDevice(), &al_v, nullptr, &m_r.view));

			VkSamplerCreateInfo al_s = mgvk::aboutVkSamplerCreateInfo();
			al_s.magFilter = VK_FILTER_LINEAR;
			al_s.minFilter = VK_FILTER_LINEAR;
			al_s.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			al_s.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			al_s.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			al_s.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
			al_s.minLod = 0.f;
			al_s.maxLod = VK_LOD_CLAMP_NONE;
			ASSERT_RESULT(vkCreateSampler(rhi->getDevice(), &al_s, nullptr, &m_r.sampler))

			ret_model.images.push_back(m_r);
			index_imageview_map.insert(std::make_pair(index, std::make_pair(m_r.view,m_r.sampler)));

			vkDestroyBuffer(rhi->getDevice(), stagingBuffer, nullptr);
			vkFreeMemory(rhi->getDevice(), stagingMemory, nullptr);
		}
		material_i.metallic_roughness = index_imageview_map[model.m_materials[i].m_pbr_metallic_roughness.m_metallic_roughness_texture.m_index].first;
		material_i.metallic_roughness_sampler = index_imageview_map[model.m_materials[i].m_pbr_metallic_roughness.m_metallic_roughness_texture.m_index].second;
		if (index_imageview_map.find(model.m_materials[i].m_normal_texture.m_index) == index_imageview_map.end()) {
			Image normal;
			int index = model.m_materials[i].m_normal_texture.m_index;
			uint32_t size = model.m_textures[index].m_width * model.m_textures[index].m_height * 4;

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingMemory;
			Mage::VulkanHelper::bufferCreationHelper(rhi.get(), size,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				stagingBuffer, stagingMemory);
			void* p;
			vkMapMemory(rhi->getDevice(), stagingMemory, 0, size, 0, &p);
			memcpy(p, model.m_textures[index].m_data.data(), size);
			vkUnmapMemory(rhi->getDevice(), stagingMemory);

			Mage::VulkanHelper::imageCreationHelper(rhi.get(), model.m_textures[index].m_width, model.m_textures[index].m_height,
				static_cast<VkFormat>(model.m_textures[index].m_format), 1, VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				normal.data, normal.memory);

			Mage::VulkanHelper::transitionImageLayout(rhi.get(), normal.data, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1);

			Mage::VulkanHelper::fromBufferToImageCopyHelper(rhi.get(), stagingBuffer, normal.data, model.m_textures[index].m_width, model.m_textures[index].m_height);

			Mage::VulkanHelper::transitionImageLayout(rhi.get(), normal.data, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1);

			VkImageViewCreateInfo al_v = mgvk::aboutVkImageViewCeateInfo();
			al_v.image = normal.data;
			al_v.format = static_cast<VkFormat>(model.m_textures[index].m_format);
			al_v.components.a = VK_COMPONENT_SWIZZLE_A;
			al_v.components.r = VK_COMPONENT_SWIZZLE_R;
			al_v.components.g = VK_COMPONENT_SWIZZLE_G;
			al_v.components.b = VK_COMPONENT_SWIZZLE_B;
			al_v.viewType = VK_IMAGE_VIEW_TYPE_2D;
			al_v.subresourceRange = mgvk::aboutVkImageSubresourceRange();
			al_v.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			al_v.subresourceRange.baseArrayLayer = 0;
			al_v.subresourceRange.baseMipLevel = 0;
			al_v.subresourceRange.layerCount = 1;
			al_v.subresourceRange.levelCount = 1;
			ASSERT_RESULT(vkCreateImageView(rhi->getDevice(), &al_v, nullptr, &normal.view));

			VkSamplerCreateInfo al_s = mgvk::aboutVkSamplerCreateInfo();
			al_s.magFilter = VK_FILTER_LINEAR;
			al_s.minFilter = VK_FILTER_LINEAR;
			al_s.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			al_s.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			al_s.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			al_s.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
			al_s.minLod = 0.f;
			al_s.maxLod = VK_LOD_CLAMP_NONE;
			ASSERT_RESULT(vkCreateSampler(rhi->getDevice(), &al_s, nullptr, &normal.sampler))

			ret_model.images.push_back(normal);
			index_imageview_map.insert(std::make_pair(index, std::make_pair(normal.view, normal.sampler)));

			vkDestroyBuffer(rhi->getDevice(), stagingBuffer, nullptr);
			vkFreeMemory(rhi->getDevice(), stagingMemory, nullptr);
		}
		material_i.normal = index_imageview_map[model.m_materials[i].m_normal_texture.m_index].first;
		material_i.normal_sampler = index_imageview_map[model.m_materials[i].m_normal_texture.m_index].second;

		VkDescriptorSetAllocateInfo set_alloc = mgvk::aboutVkDescriptorSetAllocateInfo();
		set_alloc.descriptorPool = rhi->getDescriptorPool();
		set_alloc.descriptorSetCount = 1;
		set_alloc.pSetLayouts = &layout;
		ASSERT_RESULT(vkAllocateDescriptorSets(rhi->getDevice(), &set_alloc, &material_i.set));

		VkDescriptorImageInfo image_info[3] = {};
		VkWriteDescriptorSet write[3] = {};
		for (uint32_t i{ 0 }; i < 3; ++i) {
			image_info[i] = mgvk::aboutVkDescriptorImageInfo();
			image_info[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			write[i] = mgvk::aboutVkWriteDescriptorSet();
			write[i].descriptorCount = 1;
			write[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			write[i].dstBinding = i;
			write[i].dstSet = material_i.set;
			write[i].pImageInfo = &image_info[i];
		}
		image_info[0].imageView = material_i.albedo;
		image_info[0].sampler = material_i.albedo_sampler;
		image_info[1].imageView = material_i.normal;
		image_info[1].sampler = material_i.normal_sampler;
		image_info[2].imageView = material_i.metallic_roughness;
		image_info[2].sampler = material_i.metallic_roughness_sampler;
		vkUpdateDescriptorSets(rhi->getDevice(), 3, write, 0, nullptr);

		ret_model.materials.push_back(material_i);
	}

	ret_model.meshes.resize(model.m_meshes.size());
	for (uint32_t i{ 0 }; i < model.m_meshes.size(); ++i) {
		ret_model.meshes[i].material_index = model.m_meshes[i].m_primitives.front().m_material;

		auto& primitive = model.m_meshes[i].m_primitives.front();
		if (primitive.m_attributes.find("POSITION") != primitive.m_attributes.end()) {
			ret_model.meshes[i].attributes_des[0] = std::make_tuple(model.m_accessors[primitive.m_attributes["POSITION"]].m_byte_offset +
				model.m_buffer_views[model.m_accessors[primitive.m_attributes["POSITION"]].m_buffer_view].m_byte_offset, 
				model.m_accessors[primitive.m_attributes["POSITION"]].m_count);
		}
		if (primitive.m_attributes.find("NORMAL") != primitive.m_attributes.end()) {
			ret_model.meshes[i].attributes_des[1] = std::make_tuple(model.m_accessors[primitive.m_attributes["NORMAL"]].m_byte_offset +
				model.m_buffer_views[model.m_accessors[primitive.m_attributes["NORMAL"]].m_buffer_view].m_byte_offset,
				model.m_accessors[primitive.m_attributes["NORMAL"]].m_count);
		}
		if (primitive.m_attributes.find("TANGENT") != primitive.m_attributes.end()) {
			ret_model.meshes[i].attributes_des[2] = std::make_tuple(model.m_accessors[primitive.m_attributes["TANGENT"]].m_byte_offset +
				model.m_buffer_views[model.m_accessors[primitive.m_attributes["TANGENT"]].m_buffer_view].m_byte_offset,
				model.m_accessors[primitive.m_attributes["TANGENT"]].m_count);
		}
		if (primitive.m_attributes.find("TEXCOORD_0") != primitive.m_attributes.end()) {
			ret_model.meshes[i].attributes_des[3] = std::make_tuple(model.m_accessors[primitive.m_attributes["TEXCOORD_0"]].m_byte_offset +
				model.m_buffer_views[model.m_accessors[primitive.m_attributes["TEXCOORD_0"]].m_buffer_view].m_byte_offset,
				model.m_accessors[primitive.m_attributes["TEXCOORD_0"]].m_count);
		}
		if (primitive.m_indices != -1) {
			ret_model.meshes[i].index_count = model.m_accessors[primitive.m_indices].m_count;
			ret_model.meshes[i].index_offset = model.m_accessors[primitive.m_indices].m_byte_offset + model.m_buffer_views[model.m_accessors[primitive.m_indices].m_buffer_view].m_byte_offset;
		}
	}

	_process_node(ret_model, model.m_nodes, 0, glm::mat4(1.f));

	return ret_model;
}

void run() {
	window->initialize();
	rhi->initialize(window);

	buffers.resize(rhi->getSwapchainSize());
	memories.resize(rhi->getSwapchainSize());
	for (int i{ 0 }; i < rhi->getSwapchainSize(); ++i) {
		Mage::VulkanHelper::bufferCreationHelper(
			rhi.get(), 1024 * 1024 * 20,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			buffers[i], memories[i]);
	}
	RenderModel vk_model{};
	vk_model = loadModel("E:\\Download\\makarov_pistol\\scene.gltf");

	Pipeline* pipe = new Pipeline;
	pipe->initialize(rhi);
	while (!window->shouldClose()) {
		pipe->draw(vk_model);
		window->pollEvents();
	}
}