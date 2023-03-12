#pragma once

#include<memory>

#include"core/meta/reflection/reflection.h"

namespace Mage {
	class GameObject;
	class Widget;

	REFLECTION_TYPE(Component)
	//base class of all components
	CLASS(Component, WhiteListFields) {
		REFLECTION_BODY(Component)
	protected:
		bool should_tick_in_editor{ false };
		GameObject* game_object{nullptr};
	public:
		Component() = default;
		virtual ~Component() {}

		GameObject* GetGameObject() { return game_object; }
		void SetGameObject(GameObject* parent_obj) { game_object = parent_obj; }
		bool IsTickInEditor() const { return should_tick_in_editor; }

		virtual void tick(float delta) {}

		virtual std::shared_ptr<Widget> Draw();
	};
}