#include"engine_core/scaffold/components/lightcomponent/Light_component.h"

namespace Mage {
	void LightComponent::tick(float delta) {
		//auto& pending_lights = engine_global_context.m_render_system->getPendingData()->m_lights;
		//switch (type[0]) {
		//case 'D':
		//{
		//	DirectionalLight diLi{};
		//	diLi.m_direction = game_object->GetComponent(TransformComponent)->Rotation() * Vector3::down;
		//	diLi.m_color = color;
		//	diLi.m_intensity = intensity;

		//	pending_lights.m_directional.emplace_back(diLi);
		//}
		//	break;
		//case 'P':
		//{
		//	PointLight pLi{};
		//	pLi.m_position = game_object->GetComponent(TransformComponent)->Position();
		//	pLi.m_color = color;
		//	pLi.m_intensity = intensity;

		//	pending_lights.m_point.emplace_back(pLi);
		//}
		//	break;
		//default:
		//	break;
		//}
	}

	const std::string& LightComponent::Type() const {
		return type;
	}

	const Vector3& LightComponent::Color() const {
		return color;
	}
	
	const float LightComponent::Intensity() const {
		return intensity;
	}

	void LightComponent::SetType(const std::string& ntype) {
		type = ntype;
	}

	void LightComponent::SetColor(const Vector3& ncolor) {
		color = ncolor;
	}

	void LightComponent::SetIntensity(float nint) {
		intensity = nint;
	}
}