#pragma once
#include<core/macro.h>

#include<engine_core/render_engine/render_guid.h>

#include<unordered_map>
#include<limits>
#include<deque>

namespace Mage {

	template<typename R, typename G = GUID32>
	class GUIDGenerator {
		using guid_type = G;
		using resource_type = R;
	public:
		guid_type generateGUID(const resource_type& res) {
			guid_type allocating_guid{ m_allocated_guid };
			if (m_free_guid.empty()) {
				if (!isValidGUID(allocating_guid) || m_guid_res_map.find(allocating_guid) != m_guid_res_map.end()) {
					MAGE_THROW(failed to generate a guid for resource)
				}
				m_guid_res_map.insert(std::make_pair(allocating_guid, res));
				m_res_guid_map.insert(std::make_pair(res, allocating_guid));
				++m_allocated_guid;
			}
			else {
				allocating_guid = m_free_guid.front();
				if (!isValidGUID(allocating_guid)||m_guid_res_map.find(allocating_guid) != m_guid_res_map.end()) {
					MAGE_THROW(failed to generate a guid for resource)
				}
				m_guid_res_map.insert(std::make_pair(allocating_guid, res));
				m_res_guid_map.insert(std::make_pair(res, allocating_guid));
				m_free_guid.pop_front();
			}
			return allocating_guid;
		}

		bool hasResource(const resource_type& res) { return m_res_guid_map.find(res) != m_res_guid_map.end(); }

		bool freeGUID(const guid_type& guid) {
			if (!isValidGUID(guid) || m_guid_res_map.find(guid) == m_guid_res_map.end()) {
				return false;
			}
			auto resource = m_guid_res_map[guid];
			m_res_guid_map.erase(resource);
			m_guid_res_map.erase(guid);

			m_free_guid.push_back(guid);
			return true;
		}

		bool freeResource(const resource_type& res) {
			if (m_res_guid_map.find(res) == m_res_guid_map.end()) {
				return false;
			}
			auto guid = m_res_guid_map[res];
			m_guid_res_map.erase(guid);
			m_res_guid_map.erase(res);

			m_free_guid.push_back(guid);
			return true;
		}

		void clear() {
			m_guid_res_map.clear();
			m_res_guid_map.clear();
			m_free_guid.clear();
			m_allocated_guid = 0;
		}

		static bool isValidGUID(const guid_type& guid) {
			return guid != std::numeric_limits<guid_type>::max();
		}
	private:
		std::unordered_map<guid_type, resource_type> m_guid_res_map;
		std::unordered_map<resource_type, guid_type> m_res_guid_map;

		std::deque<guid_type> m_free_guid;
		static guid_type m_allocated_guid;
	};

	template<typename R, typename G> G GUIDGenerator<R, G>::m_allocated_guid = 0;
}