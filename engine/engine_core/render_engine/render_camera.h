#pragma once
#include"core/math/vector3.h"
#include"core/math/quaternion.h"
#include"core/math/matrix4x4.h"

namespace Mage {
	class RenderCamera {
	public:
		RenderCamera() = default;
		//TODO:Ŀǰ?Ǵ?????
		Matrix4x4 getViewMatrix() = delete;
		Matrix4x4 getPerspectiveMatrix();
		Matrix4x4 getPerspectiveMatrix_Infinite();
		Matrix4x4 getPerspectiveMatrix_Precise();
		Matrix4x4 getInverseViewMatrix() = delete;
		Matrix4x4 getInversePerspectiveMatrix() = delete;

		Vector3 position() { return m_position; }
		Quaternion rotation() { return m_rotation; }

		void setPosition(const Vector3& new_p) { m_position = new_p; }
		void setPosition(float x, float y, float z) { m_position.x = x; m_position.y = y; m_position.z = z; }
		void setRotation(const Quaternion& new_r) { m_rotation = new_r; }
		void setRotation(float x, float y, float z, float w) { m_rotation.x = x; m_rotation.y = y; m_rotation.z = z; m_rotation.w = w; }

		float fov() { return m_fov; }
		float aspect() { return m_aspect; }
		float zNear() { return m_znear; }
		float zfar() { return m_zfar; }

		void setFov(float new_fov) { m_fov = new_fov; }
		void setAspect(float new_a) { m_aspect = new_a; }
		void setzNear(float new_near) { m_znear = new_near; }
		void setzFar(float new_far) { m_zfar = new_far; }

	public:
		Vector3 m_position{ Vector3::zero };
		Quaternion m_rotation{ Quaternion::identity };

		float m_fov{90.f}; //degree
		float m_aspect{ 0.f };
		float m_znear{ 0.f };
		float m_zfar{ 0.f };
	};
}