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
		T getNextProcess() { auto top = std::move(m_process_deque.front()); m_process_deque.pop_front(); return top; }
		auto size() { return m_process_deque.size(); }
		bool empty() { return m_process_deque.empty(); }
		void push_back(const T& obj) { m_process_deque.push_back(obj); }
	private:
		std::deque<T> m_process_deque;
	};

	//render scene��ΪCPU��GPU��Դͨ�ŵĽӿڣ���CPU�������Ⱦ��Դ��ʵ�ʵ���Ⱦ���ݴ洢��render resource��
	class RenderScene {
	public:
		//TODO:light
		void initialize();

		//render_model:����һ���������Ⱦʵ��
		std::vector<VkRenderModel> m_render_models;

		GUIDGenerator<VkRenderMeshURI>& getMeshGUIDGenerator();
		GUIDGenerator<VkRenderTextureURI>& getTextureGUIDGenerator();

		std::shared_ptr<SceneProcessDeque<VkRenderModelInfo>> m_p_scene_load_deque;
		std::shared_ptr<SceneProcessDeque<VkRenderModelInfo>> m_p_scene_delete_deque;

	private:
		//guid generator
		GUIDGenerator<VkRenderMeshURI>		m_mesh_guid_generator;
		GUIDGenerator<VkRenderTextureURI>	m_texture_guid_generator;
	};
}