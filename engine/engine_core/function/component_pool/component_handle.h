#pragma once

#include<core/meta/reflection/reflection.h>

#define NULL_HANDLE -1

namespace Mage {
	class ComponentPoolBase;
	class Component;

	class ComponentHandle {
		friend class ComponentPoolBase;
	public:
		ComponentHandle();
		ComponentHandle(const ComponentHandle& copy);
		ComponentHandle(ComponentHandle&& move);
		ComponentHandle(int i);

		bool isValid() const;
		Component* get() const;

		Component* operator->() const;
		Component& operator*() const;

		ComponentHandle& operator=(const ComponentHandle& copy);
		ComponentHandle& operator=(ComponentHandle&& move);

		bool operator==(const ComponentHandle& ComH);

		ComponentHandle operator++(int);
		ComponentHandle operator++();
		ComponentHandle operator--(int);
		ComponentHandle operator--();
		ComponentHandle operator+(int distance) const;
		ComponentHandle operator-(int distance) const;

		explicit operator Reflection::ReflectionPtr<Component>() const;

		friend ComponentHandle operator+(int distance, const ComponentHandle& cHandle);
	private:
		int index;
		ComponentPoolBase* pool;
	};
}