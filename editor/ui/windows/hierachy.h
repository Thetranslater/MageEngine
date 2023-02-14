#pragma once
#include<unordered_map>

#include<ui/window.h>

namespace Mage {
	class GameObject;
	//TODO:unfinish
	class Hierachy : public Window {
	public:
		Hierachy(const std::string& name);
		Hierachy(const std::string& name, const WindowConfig& config);

		void draw() override;
	private:
		void _create_callback_(GameObject& object);
		void _destroy_callback_(GameObject& object);
		std::unordered_map<const GameObject*, Widget*> object_node_map;
	};
}