#pragma once

#include<type_traits>

#include<core/macro.h>

namespace Mage {
	template<typename T>
	class Reference : public std::_Weak_types<T> {
		static_assert(std::is_object_v<T> or std::is_function_v<T>, "Reference<T> must be an obect or function type.");
	public:
		Reference() = default;
		Reference(T& val) {
			ptr = std::addressof(val);
		}

		inline constexpr bool isValid() { return ptr != nullptr; }

		constexpr operator T& () {
			if (isValid())
				return *ptr;
			MAGE_THROW(dereference invalid object)
		}

		constexpr Reference<T>& operator=(Reference<T>& ref) {
			if (ref.isValid()) {
				ptr = std::addressof(ref.get());
				return *this;
			}
			MAGE_THROW(invalid reference assignment)
		}

		constexpr Reference<T>& operator=(T& val) {
			ptr = std::addressof(val);
			return *this;
		}

		constexpr T& get() {
			if(isValid())
				return *ptr;
			MAGE_THROW(dereference invalid object)
		}

		constexpr bool isFunc() {
			const bool ret = std::is_function_v<T>;
			return ret;
		}
	private:
		T* ptr{ nullptr };
	};

	template<typename T>
	constexpr Reference<T> make_ref(T& val) {
		return Reference<T>{val};
	}
}