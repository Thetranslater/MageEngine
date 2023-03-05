#pragma once

#include<engine_core/render_engine/render_guid_generator.h>
#include<engine_core/render_engine/render_model.h>
#include<engine_core/render_engine/resource_swap_header.h>

#include<deque>

namespace Mage {
	struct DirectionalLight;
	struct PointLight;

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

	//render scene管理需要渲染的render model,作为渲染资源的一部分上下文,处理cpu和gpu资源交换。
	class RenderScene {
	public:
		RenderScene();
		//TODO:light
		void initialize();

		VkRenderModel& getRenderModel(const ID request);

		//render_model:代表一个抽象的渲染实例
		std::unordered_map<ID,VkRenderModel> m_render_models;
		std::reference_wrapper<std::vector<DirectionalLight>> m_directional_lights;
		std::reference_wrapper<std::vector<PointLight>> m_point_lights;

		GUIDGenerator<std::variant<VkRenderMeshURI, RawMeshData>>&		getMeshGUIDGenerator();
		GUIDGenerator<std::variant<VkRenderImageURI, RawImageData>>&	getImageGUIDGenerator();
		GUIDGenerator<VkRenderPartMesh>&								getPartMeshGUIDGenerator();
		GUIDGenerator<VkRenderMaterialDescription>&						getMaterialGUIDGenerator();

		std::shared_ptr<SceneProcessDeque<VkRenderModelInfo>> m_p_scene_load_deque;
		std::shared_ptr<SceneProcessDeque<VkRenderModelInfo>> m_p_scene_delete_deque;
	private:
		//guid generator
		GUIDGenerator<std::variant<VkRenderMeshURI, RawMeshData>>		m_mesh_guid_generator;
		GUIDGenerator<std::variant<VkRenderImageURI, RawImageData>>		m_image_guid_generator;
		GUIDGenerator<VkRenderPartMesh>									m_part_mesh_guid_generator;
		GUIDGenerator<VkRenderMaterialDescription>						m_material_guid_generator;
	};
}