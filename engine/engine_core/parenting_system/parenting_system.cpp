#include"engine_core/parenting_system/parenting_system.h"
#include"engine_core/function/global_context/global_context.h"
#include"engine_core/scaffold/world_manager/world_manager.h"
#include"engine_core/scaffold/world.h"
#include"engine_core/scaffold/scene.h"
#include"engine_core/scaffold/game_object.h"

namespace Mage {
	void ParentingSystem::tick(float delta) {
		auto sceneOnFlight = engine_global_context.m_world_manager->getCurrentWorld()->getCurrentScene();
		for (const auto& root : sceneOnFlight->getConstRoots()) {
			std::queue<TransformComponent*> que;
			que.push(sceneOnFlight->getGameObject(root).lock()->GetComponent(TransformComponent));
			while (not que.empty()) {
				auto front = que.front();
				que.pop();
				front->tick(delta);
				for (auto& child : front->GetChildren()) {
					que.push(child);
				}
			}
		}
	}
}