#include"engine_core/render_engine/render_camera.h"

namespace Mage {
	const Vector3 RenderCamera::LEFT = { 1.f,0.f,0.f };
	const Vector3 RenderCamera::UP = { 0.f,1.f,0.f };
	const Vector3 RenderCamera::FORWARD = { 0.f,0.f,1.f };

	Matrix4x4 RenderCamera::getViewMatrix() {
		Matrix4x4 view;
		
		Vector3 world_x = left();
		Vector3 world_y = up();
		Vector3 world_z = forward();

		view[0][0] = world_x.x;
		view[0][1] = world_x.y;
		view[0][2] = world_x.z;
		view[1][0] = world_y.x;
		view[1][1] = world_y.y;
		view[1][2] = world_y.z;
		view[2][0] = world_z.x;
		view[2][1] = world_z.y;
		view[2][2] = world_z.z;
		view[3][0] = 0.f;
		view[3][1] = 0.f;
		view[3][2] = 0.f;
		view[0][3] = -Vector3::Dot(m_position, world_x);
		view[1][3] = -Vector3::Dot(m_position, world_y);
		view[2][3] = -Vector3::Dot(m_position, world_z);
		view[3][3] = 1.f;

		return view;
	}

	Matrix4x4 RenderCamera::getPerspectiveMatrix() {
		Matrix4x4 factor{ Matrix4x4::identity };
		//camera axis reverse
		factor[0][0] = -1.f;
		//vulkan 
		factor[1][1] = -1.f;
		return factor * Matrix4x4::Perspective(m_fov, m_aspect, m_znear, m_zfar);
	}

	Matrix4x4 RenderCamera::getInversePerspectiveMatrix() {
		Matrix4x4 inv_pers{ Matrix4x4::zero };
		float tan_half_fov = Mathf::Tan(m_fov / 2.f * Mathf::Deg2Rad);
		inv_pers[0][0] = tan_half_fov * m_aspect;
		inv_pers[1][1] = tan_half_fov;
		inv_pers[2][3] = 1.f;
		inv_pers[3][2] = (m_znear - m_zfar) / (m_znear * m_zfar);
		inv_pers[3][3] = 1.f / m_znear;

		return inv_pers;
	}
	//TODO:验证
	Matrix4x4 RenderCamera::getPerspectiveMatrix_Infinite() {
		Matrix4x4 normal_pers = getPerspectiveMatrix();
		normal_pers[2][2] = 1.f - FLT_EPSILON;
		normal_pers[2][3] = -m_znear * (1.f - FLT_EPSILON);
		return normal_pers;
	}
	//TODO::验证
	Matrix4x4 RenderCamera::getPerspectiveMatrix_Precise() {
		Matrix4x4 normal_pers = getPerspectiveMatrix();
		normal_pers[2][2] = m_znear / (m_znear - m_zfar);
		normal_pers[2][3] = -normal_pers[2][3];
		return normal_pers;
	}

	void RenderCamera::rotate(float delta_yaw, float delta_pitch) {
		Quaternion pitch, yaw;
		pitch = Quaternion::AngleAxis(delta_pitch, LEFT);
		yaw = Quaternion::AngleAxis(delta_yaw, UP);

		//??
		m_rotation = yaw * m_rotation * pitch;
	}

	void RenderCamera::move(const Vector3& delta) {
		m_position += delta;
	}
}