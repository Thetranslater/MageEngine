	#pragma once
#include<glm-master/glm/glm.hpp>
#include<engine_core/render_engine/renderer/vulkanInfo.h>

#include<array>

namespace Mage {

	//according to the requirements of gltf, set the vertex attributes to this.
	//TODO:adding attributes
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec4 tangent;
		glm::vec2 texcoord_0;
		glm::vec2 texcoord_1;
		glm::vec4 color_0;

		static std::array<VkVertexInputBindingDescription, 6> getBindingDescriptions() {
			std::array<VkVertexInputBindingDescription, 6> bindings;
			for (int i = 0; i < 6; ++i) {
				bindings[i] = VulkanInfo::aboutVkVertexInputBindingDescription();
				bindings[i].binding = static_cast<uint32_t>(i);
				bindings[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			}
			//position at 0
			bindings[0].stride = sizeof(glm::vec3);
			//normal at 1
			bindings[1].stride = sizeof(glm::vec3);
			//tangent at 2
			bindings[2].stride = sizeof(glm::vec4);
			//texcoord_0 at 3
			bindings[3].stride = sizeof(glm::vec2);
			//texcoord_1 at 4
			bindings[4].stride = sizeof(glm::vec2);
			//color_0 at 5
			bindings[5].stride = sizeof(glm::vec4);

			return bindings;
		}

		static std::array<VkVertexInputAttributeDescription, 6> getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 6> attributes;
			
			for (int i = 0; i < 6; ++i) {
				attributes[i] = VulkanInfo::aboutVkVertexInputAttributeDescription();
				attributes[i].location = i;
				attributes[i].binding = i;
				attributes[i].offset = 0;
			}
			//position
			attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			//normal
			attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			//tangent
			attributes[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
			//texcoord_0
			attributes[3].format = VK_FORMAT_R32G32_SFLOAT;
			//texcoord_1
			attributes[4].format = VK_FORMAT_R32G32_SFLOAT;
			//color_0
			attributes[5].format = VK_FORMAT_R32G32B32A32_SFLOAT;
			return attributes;
		}
	};
}