#pragma once
#include<string>
#include<vector>

#include"core/meta/reflection/reflection.h"

#include"engine_core/function/id_allocator/id_allocator.h"
#include"engine_core/function/component_pool/component_handle.h"

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

		virtual void load(ObjectAsset& asset);

		virtual void tick(float delta);

		std::vector<ComponentHandle>& GetComponents() { return components; }

		template<typename component>
		component* GetComponent(const std::string& component_type) {
			for (auto& comp : components) {
				if (Reflection::ReflectionPtr<Component>{ comp }.getTypeName() == component_type) {
					return (component*)comp.get();
				}
			}
			return nullptr;
		}
#define GetComponent(ComponentType) GetComponent<ComponentType>(#ComponentType)

	private:
		GameObjectID id{ invalid_id };
		std::string go_name;

		//all objects must require a transform(components[0] == TransformComponent)
		std::vector<ComponentHandle> components;
	};
}