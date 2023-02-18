#include"engine_core/scaffold/game_object.h"
#include"engine_core/scaffold/scene.h"
#include"engine_core/scaffold/world.h"
#include"engine_core/scaffold/world_manager/world_manager.h"
#include"engine_core/scaffold/components/transformcomponent/Transform_component.h"

#include"ui/windows/hierachy.h"
#include"ui/widgets/treenode.h"
#include"context/editor_global_context.h"

namespace Mage {
	uint32_t Hierachy::id{ 0u };

	Hierachy::Hierachy(const std::string& name) : Window{name} {
		editor_global_context.m_world_manager.lock()->create_transfer.subscribe(std::bind(&Hierachy::_create_callback_, this, std::placeholders::_1));
		editor_global_context.m_world_manager.lock()->destroy_transfer.subscribe(std::bind(&Hierachy::_destroy_callback_, this, std::placeholders::_1));
		is_close = false;
	}

	Hierachy::Hierachy(const std::string& name, const WindowConfig& config) : Window{ name, config } {
		editor_global_context.m_world_manager.lock()->create_transfer.subscribe(std::bind(&Hierachy::_create_callback_, this, std::placeholders::_1));
		editor_global_context.m_world_manager.lock()->destroy_transfer.subscribe(std::bind(&Hierachy::_destroy_callback_, this, std::placeholders::_1));
		is_close = false;
	}

	void Hierachy::_create_callback_(GameObject& object) {
		std::shared_ptr<TreeNode> node{ std::make_shared<TreeNode>(object.name()) };
		//TODO:registe event
		node->clicked_event.subscribe(std::bind(&Hierachy::OnSelected, this, std::ref(object)));
		object_node_map.insert(std::make_pair(std::addressof(object), node.get()));

		if (object.GetComponent(TransformComponent)->IsChildOf()) {
			GameObject* parent = object.GetComponent(TransformComponent)->GetParent()->GetGameObject();
			object_node_map[parent]->addWidget(std::move(node));
		}
		else {
			addWidget(std::move(node));
		}
	}

	void Hierachy::_destroy_callback_(GameObject& object) {
		auto iter = object_node_map.find(&object);
		if (iter != object_node_map.end()) {
			object_node_map.erase(iter);
		}
	}

	void Hierachy::OnSelected(GameObject& selected) {
		for (const auto& [object, node] : object_node_map) {
			node->is_selected = false;
		}

		auto iter = object_node_map.find(std::addressof(selected));
		if (iter != object_node_map.end()) {
			iter->second->is_selected = true;
		}

		on_selected_event.invoke(selected);
	}

	void Hierachy::draw() {
		if (not is_close) {
			ImGuiWindowFlags flags = _convert_config_to_imgui_();

			if (!ImGui::Begin(window_id.c_str(), configuration.no_close ? nullptr : &is_close, flags)) {
				ImGui::End();
				return;
			}

			ImGui::BeginGroup();

			for (const auto& node : widgets) {
				node->draw();
			}

			ImGui::EndGroup();

			ImVec2 p = ImGui::GetWindowPos();
			position = Vector2{ p.x, p.y };
			ImVec2 s = ImGui::GetWindowSize();
			size = Vector2{ s.x,s.y };

			ImGui::End();
		}
	}
}