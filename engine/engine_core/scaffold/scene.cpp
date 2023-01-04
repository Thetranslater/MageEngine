#include"asset/scene_asset.h"

#include"engine_core/scaffold/scene.h"
#include"engine_core/scaffold/game_object.h"
#include"engine_core/scaffold/game_object_id_allocator.h"

namespace Mage {
	void Scene::clear() {
		asset_url.clear();
		name.clear();
		objects.clear();
	}

	void Scene::load(SceneAsset& asset) {
		name = asset.name;
		for (auto& object : asset.objects) {
			std::shared_ptr<GameObject> go = std::make_shared<GameObject>();
			GameObjectID id = GameObjectIDAllocator::alloc();
			go->setInstanceID(id);
			go->load(object);
			objects.insert(std::make_pair(id, go));
		}
	}

	void Scene::tick(float delta) {
		for (auto& [goid, go] : objects) {
			if (go != nullptr) {
				go->tick(delta);
			}
		}
	}
}