#pragma once

#include<queue>

namespace Mage {

	using ID = uint64_t;
	const ID invalid_id = std::numeric_limits<ID>::max();

	using GameObjectID = ID;
	using WidgetID = ID;

	class IDAllocator {
	public:
		ID alloc();
		void recycle(ID);
		void clear();
	protected:
		bool isValidID();
		bool isValidID(ID);
		std::queue<ID> m_free_id;
		ID m_allocatable_id{ 0u };
	};
}