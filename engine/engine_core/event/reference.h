#pragma once

#include<type_traits>

#include<core/macro.h>

namespace Mage {

	template<typename T>
	class Reference : public std::_Weak_types<T> {
		static_assert(std::is_object_v<T> or std::is_function_v<T>, "Reference<T> must be an obect or function type.");
	public:
		Reference() = default;
		Reference(const T& val) {
			ptr = std::addressof(const_cast<T&>(val));
		}

		inline bool isValid() { return ptr != nullptr; }

		operator T& () {
			if (isValid())
				return *ptr;
			MAGE_THROW(dereference invalid object)
		}

		Reference<T>& operator=(const Reference<T>& ref) {
			if (ref.isValid()) {
				ptr = std::addressof(ref.get());
				return *this;
			}
			MAGE_THROW(invalid referenve assignment)
		}

		T& get() {
			if(isValid())
				return *ptr;
			MAGE_THROW(dereference invalid object)
		}

	private:
		T* ptr{ nullptr };
	};

	template<typename T>
	Reference<T> make_ref(T& val) {
		return Reference<T>{val};
	}
}