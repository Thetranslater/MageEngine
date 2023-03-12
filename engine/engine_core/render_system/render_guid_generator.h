#pragma once
#include<core/macro.h>

#include<engine_core/function/id_allocator/id_allocator.h>

#include<unordered_map>
#include<limits>
#include<deque>

namespace Mage {

	template<typename R>
	class GUIDGenerator : private IDAllocator {
		using resource_type = R;
	public:
		ID generateGUID(const resource_type& res) {
			//already exsit
			if (m_res_guid_map.find(res) != m_res_guid_map.end()) {
				return m_res_guid_map[res];
			}
			ID allocating_guid{ alloc() };
			if (m_guid_res_map.find(allocating_guid) != m_guid_res_map.end()) {
				MAGE_THROW(allocating failed : duplicate id allocated)
			}
			m_guid_res_map.insert(std::make_pair(allocating_guid, res));
			m_res_guid_map.insert(std::make_pair(res, allocating_guid));
			return allocating_guid;
		}

		bool hasResource(const resource_type& res) { return m_res_guid_map.find(res) != m_res_guid_map.end(); }

		bool freeGUID(const ID& guid) {
			if (!isValidGUID(guid) || m_guid_res_map.find(guid) == m_guid_res_map.end()) {
				return false;
			}
			auto resource = m_guid_res_map[guid];
			m_res_guid_map.erase(resource);
			m_guid_res_map.erase(guid);

			recycle(guid);
			return true;
		}

		bool freeResource(const resource_type& res) {
			if (m_res_guid_map.find(res) == m_res_guid_map.end()) {
				return false;
			}
			auto guid = m_res_guid_map[res];
			m_guid_res_map.erase(guid);
			m_res_guid_map.erase(res);

			recycle(guid);
			return true;
		}

		void clear() {
			m_guid_res_map.clear();
			m_res_guid_map.clear();
		}

		static bool isValidGUID(const ID& guid) {
			return guid != std::numeric_limits<ID>::max();
		}
	private:
		std::unordered_map<ID, resource_type> m_guid_res_map;
		std::unordered_map<resource_type, ID> m_res_guid_map;
	};
}