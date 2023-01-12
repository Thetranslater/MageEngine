#pragma once
#include<string>
#include<vector>

#include"engine_core/scaffold/game_object_id_allocator.h"
#include"core/meta/reflection/reflection.h"

namespace Mage {
	//base class of all game objects
	class Component;
	class ObjectAsset;
	class GameObject {
	public:
		GameObjectID getInstanceID() { return id; }
		void setInstanceID(GameObjectID guid) { id = guid; }
		const std::string& name() { return go_name; }
		void setName(const std::string& new_name) { go_name = new_name; }

		virtual void load(ObjectAsset& asset);

		virtual void tick(float delta);

		std::vector<Reflection::ReflectionPtr<Component>>& GetComponents() { return components; }

		template<typename component>
		component* GetComponent(const std::string& component_type) {
			for (auto& comp : components) {
				if (comp.getTypeName() == component_type) {
					return (component*)comp;
				}
			}
			return nullptr;
		}
#define GetComponent(ComponentType) GetComponent<ComponentType>(#ComponentType)

	private:
		GameObjectID id{ invalid_go_id };
		std::string go_name;

		std::vector<Reflection::ReflectionPtr<Component>> components;
	};

}