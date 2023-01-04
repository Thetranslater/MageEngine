#pragma once

#include"core/meta/reflection/reflection.h"

namespace Mage {
	class GameObject;
	REFLECTION_TYPE(Component)
	//base class of all components
	CLASS(Component, WhiteListFields) {
		REFLECTION_BODY(Component)
	protected:
		bool should_tick_in_editor{ false };
		GameObject* parent{nullptr};
	public:
		Component() = default;
		virtual ~Component() {}

		void setParent(GameObject* new_parent) { parent = new_parent; }
		virtual void tick(float delta) {}
	};
}