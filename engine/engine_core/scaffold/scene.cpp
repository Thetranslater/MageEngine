#include"asset/scene_asset.h"
#include"asset/object_asset.h"

#include"engine_core/event/event.h"
#include"engine_core/scaffold/scene.h"
#include"engine_core/scaffold/game_object.h"
#include"engine_core/function/id_allocator/id_allocator.h"

namespace Mage {
	void Scene::clear() {
		asset_url.clear();
		name.clear();
		objects.clear();
	}

	GameObjectID Scene::createGameObeject(ObjectAsset& resource) {
		std::shared_ptr<GameObject> go = std::make_shared<GameObject>();
		GameObjectID id = goid_allocator.alloc();
		go->setInstanceID(id);
		go->load(resource);
		objects.insert(std::make_pair(id, go));

		create_event.invoke(*go);

		return id;
	}

	void Scene::load(SceneAsset& asset) {
		name = asset.name;
		for (auto&& object : asset.objects) {
			createGameObeject(object);
		}
	}

	void Scene::tick(float delta) {
		for (auto& [goid, go] : objects) {
			if (go != nullptr) {
				go->tick(delta);
			}
		}
	}

	IDAllocator& Scene::getGOIDAllocator() {
		return goid_allocator;
	}
}