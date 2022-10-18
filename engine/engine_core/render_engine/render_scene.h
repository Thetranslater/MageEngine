#pragma once

#include<engine_core/render_engine/render_guid_generator.h>
#include<engine_core/render_engine/render_model.h>
#include<engine_core/render_engine/render_resource.h>

namespace Mage {
	class RenderScene {
		//TODO:light

		//render_model
		std::vector<VkRenderModel> m_render_models;

		auto& getMeshGUIDGenerator();
		auto& getMaterialGUIDGenerator();

	private:
		//guid generator
		GUIDGenerator<VkRenderMeshURI>		m_mesh_guid_generator;
		GUIDGenerator<VkRenderMaterialURI>	m_material_guid_generator;
	};
}