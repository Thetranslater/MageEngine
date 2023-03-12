#pragma once
#include<unordered_map>
#include<string>
#include<memory>

#include<engine_core/function/id_allocator/id_allocator.h>
#include<engine_core/function/event/event.h>
#include<engine_core/function/event/reference.h>

namespace Mage {
	class GameObject;
	class SceneAsset;
	class ObjectAsset;
	class Scene {
	public:
		Scene();
		void load(SceneAsset& asset);
		void tick(float delta);
		void clear();
		GameObjectID createGameObeject(ObjectAsset& resource);
		IDAllocator& getGOIDAllocator();
		std::weak_ptr<GameObject> getGameObject(GameObjectID id);
		const std::vector<GameObjectID>& getConstRoots() const;

		//event
		Reference<Event<GameObject&>> create_event;
		Reference<Event<GameObject&>> destroy_event;
	private:
		std::string asset_url;
		std::string name;
		std::unordered_map<GameObjectID, std::shared_ptr<GameObject>> objects;

		IDAllocator goid_allocator;
		std::vector<GameObjectID> roots;
	};
}