#include<deque>

#include"asset/scene_asset.h"
#include"asset/object_asset.h"

#include"engine_core/function/global_context/global_context.h"
#include"engine_core/event/event.h"
#include"engine_core/event/reference.h"
#include"engine_core/scaffold/scene.h"
#include"engine_core/scaffold/game_object.h"
#include"engine_core/scaffold/components/component.h"
#include"engine_core/scaffold/world_manager/world_manager.h"
#include"engine_core/function/id_allocator/id_allocator.h"

namespace Mage {
	Scene::Scene() :
		create_event{ make_ref(engine_global_context.m_world_manager->create_transfer) },
		destroy_event{ make_ref(engine_global_context.m_world_manager->destroy_transfer) } {}

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

		create_event.get().invoke(*go);

		return id;
	}

	void Scene::load(SceneAsset& asset) {
		std::deque<int> que;
		auto createObjectsByRelation = [&](int root) {
			que.push_back(root);
			while (not que.empty()) {
				int creating{ que.front() };
				que.pop_front();
				createGameObeject(asset.objects[creating]);
				for (int child : asset.nodes[creating].children) {
					que.push_back(child);
				}
			}
		};

		name = asset.name;
		for (int root : asset.roots) {
			que.clear();
			createObjectsByRelation(root);
		}
	}

	void Scene::tick(float delta) {
		//transform tick
		for (auto& [goid, go] : objects) {
			if (go != nullptr) {
				go->GetComponents().front()->tick(delta);
			}
		}

		for (auto& [goid, go] : objects) {
			if (go != nullptr) {
				auto& components = go->GetComponents();
				for (int i{ 1 }; i < components.size(); ++i) {
					components[i]->tick(delta);
				}
			}
		}
	}

	IDAllocator& Scene::getGOIDAllocator() {
		return goid_allocator;
	}
}