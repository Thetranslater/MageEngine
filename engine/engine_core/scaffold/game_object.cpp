#include"asset/object_asset.h"

#include"core/meta/reflection/reflection.h"

#include"engine_core/scaffold/game_object.h"
#include"engine_core/scaffold/components/transformcomponent/Transform_component.h"

namespace Mage {
	GameObject::GameObject() {
		auto transform = MAGE_REFLECTION_NEW(TransformComponent);
		components.emplace_back(transform);
	}

	GameObject::GameObject(const std::string& name) :GameObject() {
		go_name = name;
	}

	void GameObject::load(ObjectAsset& asset) {
		go_name = asset.name;

		components = std::move(asset.components);
		for (auto& comp : components) {
			Component* raw_component = comp.getPtr();
			raw_component->SetGameObject(this);
		}
	}

	void GameObject::tick(float delta) {
		for (auto& comp : components) {
			comp->tick(delta);
		}
	}
}