#include"core/macro.h"

#include"engine_core/input/key_family.h"
#include"engine_core/input/input_system.h"

#include"engine_core/scaffold/components/cameracomponent/Camera_component.h"
#include"engine_core/scaffold/components/transformcomponent/Transform_component.h"
#include"engine_core/scaffold/game_object.h"

#include"engine_core/function/global_context/global_context.h"

namespace Mage {
	const Vector3 CameraComponent::UP = { 0.f,0.f,1.f };
	const Vector3 CameraComponent::LEFT = { 0.f,1.f,0.f };
	const Vector3 CameraComponent::FORWARD = { 1.f,0.f,0.f };

	void CameraComponent::tick(float delta) {
		if (should_tick_in_editor) {
			constexpr float slow_speed = 0.01f;
			constexpr float fast_speed = 1.f;

			auto input_system = engine_global_context.m_input_system;
			TransformComponent* transform = game_object->GetComponent(TransformComponent);
			if (transform == nullptr) {
				MAGE_THROW(camera must need a transform component)
			}
			Vector3 camera_position = transform->Position();
			Quaternion camera_rotation = transform->Rotation();

			//update transform

			//prepare render camera's swap data
		}
	}
}