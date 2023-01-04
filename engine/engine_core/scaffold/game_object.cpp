#include"asset/object_asset.h"

#include"engine_core/scaffold/game_object.h"
#include"engine_core/scaffold/components/component.h"

namespace Mage {
	void GameObject::load(ObjectAsset& asset) {
		go_name = asset.name;

		components = std::move(asset.components);
		for (auto& comp : components) {
			Component* raw_component = comp.getPtr();
			raw_component->setParent(this);
		}
	}

	void GameObject::tick(float delta) {
		for (auto& comp : components) {
			comp->tick(delta);
		}
	}
}