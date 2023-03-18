#pragma once

#include"core/math/aabb.h"

#include"engine_core/render_system/resource_swap_header.h"
#include"engine_core/render_system/render_resource.h"

namespace Mage {
	//this is a bad name, a gltf model create a VkRenderModelInfo which including primitives.
	struct VkRenderModelInfo {
		ID m_go_id;//TODO
		Matrix4x4 m_transform{Matrix4x4::identity};
		AxisAlignedBoundingBox m_bounding_box;
		VkRenderMeshInfo m_mesh_info{};
		VkRenderImageInfo m_images_info{};
		VkRenderMaterialInfo m_materials_info{};
	};

	struct VkRenderModel {
		VkRenderModel() = default;
		VkRenderModel(int size) :m_mesh_descriptions(size) {}

		glm::mat4x4 m_model_matrix;//user控制的transform组件，传递进GPU中。
		AxisAlignedBoundingBox m_bounding_box;

		std::vector<VkRenderMeshDescription> m_mesh_descriptions;
	};
}