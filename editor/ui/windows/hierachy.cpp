#include"engine_core/scaffold/game_object.h"
#include"engine_core/scaffold/scene.h"
#include"engine_core/scaffold/world.h"
#include"engine_core/scaffold/world_manager/world_manager.h"
#include"engine_core/function/global_context/global_context.h"

#include"ui/windows/hierachy.h"
#include"ui/widgets/treenode.h"

namespace Mage {
	Hierachy::Hierachy(const std::string& name) : Window{name} {
		auto active_scene = engine_global_context.m_world_manager->getCurrentWorld()->getCurrentScene();
		active_scene->create_event.subscribe(std::bind(&Hierachy::_create_callback_, this, std::placeholders::_1));
		active_scene->destroy_event.subscribe(std::bind(&Hierachy::_destroy_callback_, this, std::placeholders::_1));
	}

	Hierachy::Hierachy(const std::string& name, const WindowConfig& config) : Window{ name, config } {
		auto active_scene = engine_global_context.m_world_manager->getCurrentWorld()->getCurrentScene();
		active_scene->create_event.subscribe(std::bind(&Hierachy::_create_callback_, this, std::placeholders::_1));
		active_scene->destroy_event.subscribe(std::bind(&Hierachy::_destroy_callback_, this, std::placeholders::_1));
	}

	void Hierachy::_create_callback_(GameObject& object) {
		std::shared_ptr<TreeNode> node{ std::make_shared<TreeNode>(object.name()) };
		//TODO:registe event

		addWidget(node);
		object_node_map.insert(std::make_pair(&object, node.get()));
	}

	void Hierachy::_destroy_callback_(GameObject& object) {
		auto iter = object_node_map.find(&object);
		if (iter != object_node_map.end()) {
			object_node_map.erase(iter);
		}
	}

	void Hierachy::draw() {
		ImGuiWindowFlags flags = _convert_config_to_imgui_();

		if (!ImGui::Begin("HierachyWindow", configuration.no_close ? nullptr : &is_close, flags)) {
			ImGui::End();
			return;
		}

		ImGui::BeginGroup();

		for (const auto& node : widgets) {
			node->draw();
		}

		ImGui::EndGroup();

		ImGui::End();
	}
}