#pragma once

#include<engine_core/render_engine/render_guid_generator.h>
#include<engine_core/render_engine/render_model.h>
#include<engine_core/render_engine/render_resource.h>

#include<deque>

namespace Mage {
	template<typename T>
	class SceneProcessDeque {
		using process_type = T;
	public:
		T& getNextProcess() { auto& top = m_process_deque.front(); m_process_deque.pop_front(); return top; }
		auto size() { return m_process_deque.size(); }
		bool empty() { return m_process_deque.empty(); }
		void push_back(const T& obj) { m_process_deque.push_back(obj); }
	private:
		std::deque<T> m_process_deque;
	};

	class RenderScene {
	public:
		//TODO:light

		//render_model
		std::vector<VkRenderModel> m_render_models;

		auto& getMeshGUIDGenerator();
		auto& getMaterialGUIDGenerator();

		std::shared_ptr<SceneProcessDeque<VkRenderModelInfo>> m_p_scene_load_deque;
		std::shared_ptr<SceneProcessDeque<VkRenderModelInfo>> m_p_scene_delete_deque;

	private:
		//guid generator
		GUIDGenerator<VkRenderMeshURI>		m_mesh_guid_generator;
		GUIDGenerator<VkRenderMaterialURI>	m_material_guid_generator;
	};
}