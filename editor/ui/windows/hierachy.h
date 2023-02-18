#pragma once
#include<unordered_map>

#include<ui/window.h>

#include<engine_core/event/event.h>

namespace Mage {
	class GameObject;
	class TreeNode;
	//TODO:unfinish
	class Hierachy : public Window {
	public:
		Hierachy(const std::string& name);
		Hierachy(const std::string& name, const WindowConfig& config);

		void OnSelected(GameObject& selected);

		void draw() override;
	private:
		void _create_callback_(GameObject& object);
		void _destroy_callback_(GameObject& object);
		std::unordered_map<const GameObject*, TreeNode*> object_node_map;

		static uint32_t id;

	public:
		Event<GameObject&> on_selected_event;
	};
}