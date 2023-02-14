#include<stdexcept>

#include"engine_core/function/id_allocator/id_allocator.h"

namespace Mage {
	bool IDAllocator::isValidID() {
		return m_allocatable_id != invalid_id;
	}

	bool IDAllocator::isValidID(ID id) {
		return id != invalid_id;
	}

	ID IDAllocator::alloc() {
		if (isValidID()) {
			ID ret{};
			if (not m_free_id.empty()) {
				ret = m_free_id.front();
				m_free_id.pop();
			}
			else {
				ret = m_allocatable_id++;
			}
			return ret;
		}
		throw std::range_error("id allocator's allocatable id has reach to the up limit");
	}

	void IDAllocator::recycle(ID id) {
		if (isValidID(id)) {
			m_free_id.push(id);
		}
	}

	void IDAllocator::clear() {
		while (not m_free_id.empty()) m_free_id.pop();
		m_allocatable_id = 0u;
	}
}