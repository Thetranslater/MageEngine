#pragma once
#include<atomic>
#include<limits>

#include"core/macro.h"

namespace Mage {
	using GameObjectID = uint64_t;
	constexpr GameObjectID invalid_go_id = std::numeric_limits<GameObjectID>::max();

	class GameObjectIDAllocator {
	public:
		static GameObjectID alloc();
	private:
		static std::atomic<GameObjectID> id;
	};

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