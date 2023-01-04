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
}