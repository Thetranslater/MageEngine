#pragma once

namespace Mage {
	class Component;

	class ComponentPoolBase {
		template<typename Ty, typename Te>
		friend class ComponentHandle;
		friend class ComponentHandler;
	public:
		int size() const { return _size; }
		int capacity() const { return _capacity; }
		virtual ComponentHandler allocate(int num = 1) = 0;
	protected:
		virtual Component* handleAccess(const void*) = 0;
		int _size{ 0 };
		int _capacity{ 0 };
	};
}
