#pragma once

#include<vector>
#include<type_traits>
#include<cassert>

#include<engine_core/function/component_pool/component_buffer.h>

#define NULL_HANDLE -1

namespace Mage {
	class ComponentHandle;
	class Component;

	class ComponentPoolBase {
		friend class ComponentHandle;
	public:
		ComponentPoolBase();
		virtual ~ComponentPoolBase() {};
		//TODO:iterator
		ComponentHandle begin();
		ComponentHandle end();
		const ComponentHandle cbegin();
		const ComponentHandle cend();

		virtual ComponentHandle allocate(int num = 1) = 0;
		virtual bool release(ComponentHandle& handle) = 0;
		//function do not check the validation of index in runtime, make sure that every access is valid when debug
		virtual Component* access(const ComponentHandle& handle) = 0;

		int capacity() const;
		int size() const;
	protected:
		int _size;
		int _capacity; //must be multiple of MAX_COMPONENT_CONTAIN
	};
}