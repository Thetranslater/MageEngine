#include"engine_core/scaffold/game_object_id_allocator.h"

namespace Mage {
	std::atomic<GameObjectID> GameObjectIDAllocator::id{ 0 };

	GameObjectID GameObjectIDAllocator::alloc() {
		auto next_id = id.load();
		id++;
		if (id >= invalid_go_id) {
			MAGE_THROW(goid overflow!)
		}
		return next_id;
	}
}