#pragma once
#include<string>
#include<vector>

#include"core/meta/reflection/reflection.h"

#include"engine_core/function/id_allocator/id_allocator.h"
#include"engine_core/function/component_pool/component_pool.h"

namespace Mage {
	//base class of all game objects
	class Component;
	class ObjectAsset;

	class GameObject {
	public:
		GameObject();
		GameObject(const std::string& name);

		GameObjectID getInstanceID() const { return id; }
		void setInstanceID(GameObjectID guid) { id = guid; }
		const std::string& name() const { return go_name; }
		void setName(const std::string& new_name) { go_name = new_name; }
		bool isDirty() const { return is_dirty; }
		void setDirty() { is_dirty = true; }

		virtual void load(ObjectAsset& asset);

		virtual void tick(float delta);

		void PushComponent(const ComponentHandler& handler);
		void PushComponent(ComponentHandler&& handler);

		std::vector<ComponentHandler>& GetComponents() { return components; }

		template<typename component>
		component* GetComponent(const std::string& component_type) {
			for (const auto& comp : components) {
				if (comp->getTypeName() == component_type) {
					return (component*)(comp);
				}
			}
			return nullptr;
		}
#define GetComponent(ComponentType) GetComponent<ComponentType>(#ComponentType)

	private:
		GameObjectID id{ invalid_id };
		std::string go_name;
		bool is_dirty{ false };

		//all objects must require a transform(components[0] == TransformComponent)
		std::vector<ComponentHandler> components;
	};
}