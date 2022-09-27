	#pragma once
#include<glm-master/glm/glm.hpp>
#include<engine_core/render_engine/renderer/vulkanInfo.h>

#include<array>

namespace Mage {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 uv;

		static std::array<VkVertexInputBindingDescription, 1> getBindingDescriptions() {
			std::array<VkVertexInputBindingDescription, 1> bindings;
			bindings[0] = VulkanInfo::aboutVkVertexInputBindingDescription();
			bindings[0].binding = 0;
			bindings[0].stride = sizeof(Vertex);
			bindings[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindings;
		}

		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 3> attributes;
			attributes[0] = VulkanInfo::aboutVkVertexInputAttributeDescription();

			attributes[0].binding = 0;
			attributes[0].location = 0;
			attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributes[0].offset = offsetof(Vertex, position);

			attributes[1].binding = 0;
			attributes[1].location = 1;
			attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributes[1].offset = offsetof(Vertex, color);

			attributes[2].binding = 0;
			attributes[2].location = 2;
			attributes[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributes[2].offset = offsetof(Vertex, uv);

			return attributes;
		}
	};
}