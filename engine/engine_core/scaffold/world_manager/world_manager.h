#pragma once
#include<unordered_map>
#include<memory>

#define MAGE_DEFAUT_WORLD "E:/Mage/engine/asset/data/world/default_world.json"

namespace Mage {
	class World;
	class WorldManager {
	public:
		WorldManager() = default;
		void initialize();
		void clear();

		void tick(float delta);

	private:
		void loadWorldAsset(const std::string& url);

		bool is_any_world_loaded{ false };
		std::string current_load_world;
		std::unordered_map<std::string, std::shared_ptr<World>> worlds;

		std::weak_ptr<World> current_world;
	};
}