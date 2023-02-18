#pragma once
#include<unordered_map>
#include<memory>

#include<engine_core/event/event.h>

#define MAGE_DEFAUT_WORLD "E:/Mage/engine/asset/data/world/default_world.json"

namespace Mage {
	class World;
	class GameObject;
	class WorldManager {
	public:
		WorldManager() = default;
		void initialize();
		void clear();

		void tick(float delta);

		std::shared_ptr<World> getCurrentWorld() const;


		Event<GameObject&> create_transfer;
		Event<GameObject&> destroy_transfer;
	private:
		void loadWorldAsset(const std::string& url);

		bool is_any_world_loaded{ false };
		std::string current_load_world;
		std::unordered_map<std::string, std::shared_ptr<World>> worlds;

		std::weak_ptr<World> current_world;
	};
}