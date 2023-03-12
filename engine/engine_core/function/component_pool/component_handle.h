#pragma once

#include<stdexcept>

#include<core/meta/reflection/reflection.h>

#include<engine_core/function/component_pool/component_pool_base.h>

#define NULL_HANDLE -1

namespace Mage {
	template<typename Ty, typename Te>
	class ComponentPool;
	template<typename Ty, typename Te>
	class ComponentHandle;
	class Component;

	//declear
	class ComponentHandler {
		friend class ComponentPoolBase;
	public:
		ComponentHandler();
		ComponentHandler(int i, ComponentPoolBase* p);
		template<typename Uty, typename Te = std::enable_if_t<is_safely_castable<Uty*, Component*>::value>>
		ComponentHandler(const ComponentHandle<Uty, Te>& copy);
		template<typename Uty, typename Te = std::enable_if_t<is_safely_castable<Uty*, Component*>::value>>
		ComponentHandler(ComponentHandle<Uty, Te>&& move);

		template<typename Uty, typename Te = std::enable_if_t<is_safely_castable<Uty*, Component*>::value>>
		ComponentHandler& operator=(const ComponentHandle<Uty, Te>& copy);
		template<typename Uty, typename Te = std::enable_if_t<is_safely_castable<Uty*, Component*>::value>>
		ComponentHandler& operator=(ComponentHandle<Uty, Te>&& move);

		template<typename Uty, typename Te = std::enable_if_t<is_safely_castable<Component*, Uty*>::value>>
		operator ComponentHandle<Uty, Te>() const;

		explicit operator Reflection::ReflectionPtr<Component>() const;
		operator Component* () const;

		template<typename Uty, typename = std::enable_if_t<is_safely_castable<Component*, Uty*>::value>>
		operator Uty* () const;

		Component* operator->() const;
		Component& operator*() const;
		bool operator==(const ComponentHandler& r) const;

		ComponentHandler operator++(int);
		ComponentHandler& operator++();
		ComponentHandler operator--(int);
		ComponentHandler& operator--();

		ComponentHandler operator+(int distance) const;
		ComponentHandler operator-(int distance) const;
		friend ComponentHandler operator+(int distance, const ComponentHandler& handle);
	protected:
		int index{ NULL_HANDLE };
		ComponentPoolBase* pool{ nullptr };
	};

	template<typename Ty, typename Te = std::enable_if_t<std::is_convertible_v<Ty*, Component*>>>
	class ComponentHandle : protected ComponentHandler {
		friend class ComponentPool<Ty, Te>;
	public:
		ComponentHandle() = default;
		ComponentHandle(int i, ComponentPoolBase* p) : ComponentHandler(i, p) {}
		ComponentHandle(const ComponentHandler& copy) : index{ copy.index }, pool{ copy.pool } {}
		ComponentHandle(ComponentHandler&& move) : index{ move.index }, pool{ move.pool } { move.index = NULL_HANDLE; move.pool = nullptr; }
		ComponentHandle(const ComponentHandle& copy) : index{ copy.index }, pool{ copy.pool } {}
		ComponentHandle(ComponentHandle&& move) : index{ move.index }, pool{ move.pool } { move.index = NULL_HANDLE; move.pool = nullptr; }

		ComponentHandle& operator=(const ComponentHandler& copy) {
			if (pool == nullptr or copy.pool != pool) {
				throw std::runtime_error("invalid assign function call due to unmatched type");
			}
			index = copy.index;
			pool = copy.pool;
			return *this;
		}
		ComponentHandle& operator=(ComponentHandler&& move) {
			if (pool == nullptr or move.pool != pool) {
				throw std::runtime_error("invalid assign function call due to unmatched type");
			}
			index = move.index;
			pool = move.pool;
			move.index = NULL_HANDLE;
			move.pool = nullptr;
			return *this;
		}
		ComponentHandle& operator=(const ComponentHandle& copy) {
			index = copy.index;
			pool = copy.pool;
			return *this;
		}
		ComponentHandle& operator=(ComponentHandle&& move) {
			index = move.index;
			pool = move.pool;
			move.index = NULL_HANDLE;
			move.pool = nullptr;
			return *this;
		}

		operator ComponentHandler() const {
			return ComponentHandler{ index, pool };
		}

		operator Reflection::ReflectionPtr<Ty>() const {
			Component* instance{ pool->handleAccess(this) };
			return Reflection::ReflectionPtr<Ty>{instance->getTypeName(), instance};
		}
		operator Ty* () const {
			return static_cast<Ty*>(pool->handleAccess(this));
		}

		Ty* operator->() const {
			return static_cast<Ty*>(pool->handleAccess(this));
		}
		Ty& operator*() const {
			return *static_cast<Ty*>(pool->handleAccess(this));
		}
		bool operator==(const ComponentHandle& r) const { return r.index == index and r.pool == pool; }
		bool operator!=(const ComponentHandle& r) const { return !(r.index == index && r.pool == pool); }

		ComponentHandle operator++(int) { ComponentHandle ret{ index, pool }; ++index; return ret; }
		ComponentHandle& operator++() { ++index; return *this; }
		ComponentHandle operator--(int) { ComponentHandle ret{ index, pool }; --index; return ret; }
		ComponentHandle& operator--() { --index; return *this; }

		ComponentHandle operator+(int distance) const {
			assert((index + distance) >= 0);
			return ComponentHandle{ index + distance, pool };
		}
		ComponentHandle operator-(int distance) const {
			assert((index - distance) >= 0);
			return ComponentHandle{ index - distance, pool };
		}
		friend ComponentHandle operator+(int distance, const ComponentHandle& handle) {
			return handle + distance;
		}
	};

	template<typename Uty, typename Te>
	ComponentHandler::ComponentHandler(const ComponentHandle<Uty, Te>& copy) : index{ copy.index }, pool{ copy.pool } {}
	template<typename Uty, typename Te>
	ComponentHandler::ComponentHandler(ComponentHandle<Uty, Te>&& move) : index{ move.index }, pool{ move.pool } { move.index = NULL_HANDLE; move.pool = nullptr; }
	template<typename Uty, typename Te>
	ComponentHandler& ComponentHandler::operator=(const ComponentHandle<Uty, Te>& copy) {
		index = copy.index;
		pool = copy.pool;
		return *this;
	}
	template<typename Uty, typename Te>
	ComponentHandler& ComponentHandler::operator=(ComponentHandle<Uty, Te>&& move) {
		index = move.index;
		pool = move.pool;
		move.index = NULL_HANDLE;
		move.pool = nullptr;
		return *this;
	}
	template<typename Uty, typename Te>
	ComponentHandler::operator ComponentHandle<Uty, Te>() const {
		return ComponentHandle<Uty, Te>{index, pool};
	}

	template<typename Uty, typename>
	ComponentHandler::operator Uty* () const {
		return static_cast<Uty*>(pool->handleAccess(this));
	}
}