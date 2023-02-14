#include"asset/world_asset.h"
#include"asset/resource_manager/resource_manager.h"

#include"engine_core/scaffold/world.h"
#include"engine_core/scaffold/scene.h"
#include"engine_core/function/global_context/global_context.h"


namespace Mage {
	void World::load(const WorldAsset& asset) {
		is_any_scene_loaded = false;
		for (auto& url : asset.scene_urls) {
			scenes[url] = nullptr;
		}
		current_loaded_scene = asset.scene_urls[asset.default_scene];
		name = asset.name;
	}

	void World::clear() {
		is_any_scene_loaded = false;
		name.clear();
		asset_url.clear();
		current_loaded_scene.clear();
		for (auto& [url,scene] : scenes) {
			scene->clear();
		}
		scenes.clear();
		current_scene.reset();
	}

	void World::tick(float delta) {
		if (not is_any_scene_loaded) {
			loadSceneAsset(current_loaded_scene);
		}

		std::shared_ptr<Scene> update_scene = current_scene.lock();
		if (update_scene != nullptr) {
			update_scene->tick(delta);
		}
	}

	void World::loadSceneAsset(const std::string& url) {
		auto& resource_loader = engine_global_context.m_resource_manager;
		SceneAsset asset;
		std::string err;
		bool load_res = resource_loader->loadAsset(url, &asset, &err, nullptr);
		if (not load_res) {
			MAGE_THROW(failed to load current scene)
		}

		std::shared_ptr<Scene> current_default_scene = std::make_shared<Scene>();
		current_default_scene->load(asset);
		scenes[url] = current_default_scene;

		current_scene = current_default_scene;

		is_any_scene_loaded = true;
	}

	std::shared_ptr<Scene> World::getCurrentScene() const {
		return current_scene.lock();
	}
}