#include"asset/resource_manager/resource_manager.h"

#include"engine_core/scaffold/world_manager/world_manager.h"
#include"engine_core/scaffold/world.h"
#include"engine_core/function/global_context/global_context.h"

namespace Mage {
	void WorldManager::initialize() {
		is_any_world_loaded = false;
		current_load_world = MAGE_DEFAUT_WORLD;
	}

	void WorldManager::clear() {
		is_any_world_loaded = false;
		current_load_world.clear();

		for (auto& [url, world] : worlds) {
			world->clear();
		}
		worlds.clear();
		current_world.reset();
	}

	void WorldManager::tick(float delta) {
		if (not is_any_world_loaded) {
			loadWorldAsset(current_load_world);
		}
		std::shared_ptr<World> update_world = current_world.lock();
		if (update_world != nullptr) {
			update_world->tick(delta);
		}
	}

	void WorldManager::loadWorldAsset(const std::string& url) {
		auto& resource_loader = engine_global_context.m_resource_manager;
		WorldAsset asset;
		std::string err;
		bool load_res = resource_loader->loadAsset(url, &asset, &err, nullptr);
		if (not load_res) {
			MAGE_THROW(failed to load world)
		}
		
		std::shared_ptr<World> current_world = std::make_shared<World>();
		current_world->load(asset);
		worlds.insert(std::make_pair(url, current_world));

		is_any_world_loaded = true;
	}
}