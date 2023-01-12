#include"engine_core/render_engine/render_camera.h"

namespace Mage {
	Matrix4x4 RenderCamera::getViewMatrix() {
		return Matrix4x4::TRS(m_position, m_rotation, Vector3::one);
	}

	Matrix4x4 RenderCamera::getPerspectiveMatrix() {
		return Matrix4x4::Perspective(m_fov, m_aspect, m_znear, m_zfar);
	}

	Matrix4x4 RenderCamera::getInverseViewMatrix() {
		float dx = m_rotation.x + m_rotation.x;//2x
		float dy = m_rotation.y + m_rotation.y;//2y
		float dz = m_rotation.z + m_rotation.z;//2z
		float dxw = dx * m_rotation.w;//2xw
		float dyw = dy * m_rotation.w;//2yw
		float dzw = dz * m_rotation.w;//2zw
		float dxx = dx * m_rotation.x;//2x^2
		float dyy = dy * m_rotation.y;//2y^2
		float dzz = dz * m_rotation.z;//2z^2
		float dxy = dx * m_rotation.y;//2xy
		float dxz = dx * m_rotation.z;//2xz
		float dyz = dy * m_rotation.z;//2yz

		Matrix4x4 inverse_affine;
		inverse_affine[0][0] = 1 - dyy - dzz;
		inverse_affine[0][1] = dxy + dzw;
		inverse_affine[0][2] = dxz - dyw;
		inverse_affine[0][3] = -(m_position.x * inverse_affine[0][0] + m_position.y * inverse_affine[0][1] + m_position.z * inverse_affine[0][2]);
		inverse_affine[1][0] = dxy - dzw;
		inverse_affine[1][1] = 1 - dxx - dzz;
		inverse_affine[1][2] = dyz + dxw;
		inverse_affine[1][3] = -(m_position.x * inverse_affine[1][0] + m_position.y * inverse_affine[1][1] + m_position.z * inverse_affine[1][2]);
		inverse_affine[2][0] = dxz + dyw;
		inverse_affine[2][1] = dyz - dxw;
		inverse_affine[2][2] = 1 - dxx - dyy;
		inverse_affine[2][3] = -(m_position.x * inverse_affine[2][0] + m_position.y * inverse_affine[2][1] + m_position.z * inverse_affine[2][2]);
		inverse_affine[3][0] = 0.f;
		inverse_affine[3][1] = 0.f;
		inverse_affine[3][2] = 0.f;
		inverse_affine[3][3] = 1.f;

		return inverse_affine;
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

	Matrix4x4 RenderCamera::getPerspectiveMatrix_Infinite() {
		Matrix4x4 normal_pers = Matrix4x4::Perspective(m_fov, m_aspect, m_znear, m_zfar);
		normal_pers[2][2] = 1.f - FLT_EPSILON;
		normal_pers[2][3] = -m_znear * (1.f - FLT_EPSILON);
		return normal_pers;
	}

	Matrix4x4 RenderCamera::getPerspectiveMatrix_Precise() {
		Matrix4x4 normal_pers = Matrix4x4::Perspective(m_fov, m_aspect, m_znear, m_zfar);
		normal_pers[2][2] = m_znear / (m_znear - m_zfar);
		normal_pers[2][3] = -normal_pers[2][3];
		return normal_pers;
	}
}