#include"asset/object_asset.h"

#include"core/meta/reflection/reflection.h"

#include"engine_core/scaffold/game_object.h"
#include"engine_core/scaffold/components/transformcomponent/Transform_component.h"
#include"engine_core/util/util_global_context.h"

namespace Mage {
	GameObject::GameObject() {}

	GameObject::GameObject(const std::string& name) :GameObject() {
		go_name = name;
	}

	void GameObject::load(ObjectAsset& asset) {
		go_name = asset.name;

		//TODO:reflection_ptr -> componenthandler. Call allocation from component pool;
		components.resize(asset.components.size());
		for (int i{ 0 }; i < asset.components.size(); ++i) {
			const std::string type{ asset.components[i].getTypeName() };
			Reflection::TypeMeta meta{ Reflection::TypeMeta::newMetaFromName(type) };
			Reflection::FieldAccessor* accessors;
			int fields_count = meta.getFieldsList(accessors);

			components[i] = Util::MakeComponent(type);
			for (int j{ 0 }; j < fields_count; ++j) {
				accessors->set(components[i], accessors->get(asset.components[i].getPtr()));
			}
		}

		asset.components.clear();

		for (auto& comp : components) {
			comp->SetGameObject(this);
		}

		is_dirty = true;
	}

	void GameObject::tick(float delta) {
		for (auto& comp : components) {
			comp->tick(delta);
		}
	}

	void GameObject::PushComponent(const ComponentHandler& handler) {
		components.emplace_back(handler);
		is_dirty = true;
	}

	void GameObject::PushComponent(ComponentHandler&& handler) {
		components.emplace_back(std::move(handler));
		is_dirty = true;
	}
}