
#include"engine_core/render_engine/render_pending_data.h"
#include"engine_core/render_engine/render_system.h"
#include"engine_core/function/global_context/global_context.h"
#include"engine_core/scaffold/components/transformcomponent/Transform_component.h"
#include"engine_core/scaffold/components/lightcomponent/Light_component.h"
#include"engine_core/scaffold/game_object.h"

namespace Mage {
	void LightComponent::tick(float delta) {
		auto& pending_lights = engine_global_context.m_render_system->getPendingData()->m_lights;
		switch (Type[0]) {
		case 'D':
		{
			DirectionalLight diLi{};
			diLi.m_direction = game_object->GetComponent(TransformComponent)->Rotation() * Vector3::down;
			diLi.m_color = Color;
			diLi.m_intensity = Intensity;

			pending_lights.m_directional.emplace_back(diLi);
		}
			break;
		case 'P':
		{
			PointLight pLi{};
			pLi.m_position = game_object->GetComponent(TransformComponent)->Position();
			pLi.m_color = Color;
			pLi.m_intensity = Intensity;

			pending_lights.m_point.emplace_back(pLi);
		}
			break;
		default:
			break;
		}
	}
}