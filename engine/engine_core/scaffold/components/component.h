#pragma once

#include"core/meta/reflection/reflection.h"

namespace Mage {
	class GameObject;
	REFLECTION_TYPE(Component)
	//base class of all components
	CLASS(Component, WhiteListFields) {
		REFLECTION_BODY(Component)
	protected:
		GameObject* parent{nullptr};
	public:
		Component() = default;
		virtual ~Component() {}
		virtual void tick(float delta) {}
	};
}