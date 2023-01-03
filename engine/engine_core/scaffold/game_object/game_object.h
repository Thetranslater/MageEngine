#pragma once
#include<string>
#include<vector>

#include"engine_core/scaffold/game_object/game_object_id_allocator.h"
#include"core/meta/reflection/reflection.h"

namespace Mage {
	//base class of all game objects
	class Component;
	class GameObject {
	public:
		GameObjectID GetInstanceID() { return id; }
		const std::string& Name() { return name; }
		void Name(const std::string& new_name) { name = new_name; }

		virtual void tick(float delta);

		std::vector<Reflection::ReflectionPtr<Component>>& GetComponents() { return components; }

		template<typename component>
		component* GetComponent(const std::string& component_type) {
			for (auto& comp : components) {
				if (comp.getTypeName() == component_type) {
					return (component*)comp;
				}
			}
		}
#define GetComponent(ComponentType) GetComponent<ComponentType>(#ComponentType)

	private:
		GameObjectID id{ invalid_go_id };
		std::string name;

		std::vector<Reflection::ReflectionPtr<Component>> components;
	};

}