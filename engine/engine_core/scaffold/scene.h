#pragma once
#include<unordered_map>
#include<string>
#include<memory>

#include"engine_core/scaffold/game_object_id_allocator.h"

namespace Mage {
	class GameObject;
	class SceneAsset;
	class Scene {
	public:
		Scene() = default;
		void load(SceneAsset& asset);
		void tick(float delta);
		void clear();
	private:
		std::string asset_url;
		std::string name;
		std::unordered_map<GameObjectID, std::shared_ptr<GameObject>> objects;
	};
}