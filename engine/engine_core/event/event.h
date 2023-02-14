#pragma once

#include<functional>
#include<unordered_map>
#include<stdexcept>

namespace Mage {

	using ListenerID = uint64_t;
	const ListenerID invalid_listener_id = std::numeric_limits<ListenerID>::max();

	template<typename ...argTypes>
	class Event {
		using Callback = std::function<void(argTypes...)>;
	public:
		inline ListenerID subscribe(const Callback& listener) {
			if (isValidID()) {
				ListenerID arg_id = dispatch_id++;
				m_listeners.insert(std::make_pair(arg_id, listener));
				return arg_id;
			}
			throw std::range_error("event's listenerID has reach the up limit");
		}

		inline void unsubcribe(ListenerID id) {
			if (isValidID(id)) {
				m_listeners.erase(id);
			}
		}

		inline void invoke(argTypes... args) const {
			for (auto& [id, listener] : m_listeners) {
				listener(args...);
			}
		}

		inline void clear() {
			dispatch_id = 0u;
			m_listeners.clear();
		}

		inline ListenerID operator+=(const Callback& listener) {
			return subscribe(listener);
		}

		inline void operator-=(ListenerID id) {
			unsubcribe(id);
		}
	protected:
		inline bool isValidID() const {
			return dispatch_id != invalid_listener_id;
		}

		inline bool isValidID(ListenerID id) const {
			return id != invalid_listener_id;
		}

		std::unordered_map<ListenerID, Callback> m_listeners;
		ListenerID dispatch_id{ 0u };
	};
}