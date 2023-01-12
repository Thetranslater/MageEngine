#pragma once

#include"core/math/vector3.h"
#include"core/math/quaternion.h"
#include"core/math/matrix4x4.h"

#include"engine_core/scaffold/components/component.h"

namespace Mage {
	REFLECTION_TYPE(CameraComponent)
	CLASS(CameraComponent:public Component, WhiteListFields) {
		REFLECTION_BODY(CameraComponent)
	public:
		CameraComponent() { should_tick_in_editor = true; }

		void tick(float delta) override;

		float FOV() { return field_of_view; }
		float NearClipPlane() { return near_clip_plane; }
		float FarClipPlane() { return far_clip_plane; }
		//TODO:aspect

		Matrix4x4 GetViewMatrix();
		Matrix4x4 GetInvViewMatrix();
		Matrix4x4 GetPerspectiveMatrix();
		Matrix4x4 GetInvPerspectiveMatrix();
	protected:
		META(Enable)
		float field_of_view;
		META(Enable)
		float near_clip_plane;
		META(Enable)
		float far_clip_plane;

		static const Vector3 FORWARD;
		static const Vector3 UP;
		static const Vector3 LEFT;
	};
}