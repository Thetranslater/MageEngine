#pragma once
#include<unordered_map>
#include<memory>
#include<string>

namespace Mage {
	class Scene;
	class WorldAsset;

	class World {
	public:
		World() = default;
		void load(const WorldAsset& asset);

		void tick(float delta);
		void clear();

		void setName(const std::string& new_name) { name = new_name; }
		std::string getName() { return name; }
	private:
		void loadSceneAsset(const std::string& url);

		std::string asset_url;
		std::string name;
		std::string current_loaded_scene;
		std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;

		bool is_any_scene_loaded;
		std::weak_ptr<Scene> current_scene;
		
	};
}