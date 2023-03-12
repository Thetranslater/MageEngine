#pragma once

#include<vector>

#include<core/math/vector3.h>
#include<core/math/quaternion.h>


namespace Mage {
	struct DirectionalLight {
		Vector3 m_direction;
		Vector3 m_color;
		float m_intensity;
	};

	struct PointLight {
		Vector3 m_position;
		Vector3 m_color;
		float m_intensity;
	};

	struct RenderPendingLights {
		std::vector<DirectionalLight> m_directional;
		std::vector<PointLight> m_point;
	};

	struct RenderPendingCamera {
		Vector3 m_pending_position;
		Quaternion m_pending_rotation;

		float m_pending_fov;
		float m_pending_aspect;
		float m_pending_znear;
		float m_pending_zfar;
	};

	struct RenderPendingEditor {
		float viewport_width;
		float viewport_height;
		float viewport_x;
		float viewport_y;
	};

	struct RenderPendingData {
		//single frame data
		RenderPendingLights m_lights;
		RenderPendingCamera m_camera;
		RenderPendingEditor m_editor;
	};
}