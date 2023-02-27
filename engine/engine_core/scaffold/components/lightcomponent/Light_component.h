#pragma once

#include<core/math/vector3.h>
#include<core/meta/reflection/reflection.h>

#include<engine_core/scaffold/components/component.h>

#include<engine_core/scaffold/components/lightcomponent/Light_common.h>

namespace Mage {
	REFLECTION_TYPE(LightComponent);
	CLASS(LightComponent : public Component, WhiteListFields) {
		REFLECTION_BODY(LightComponent);
	public:
		LightComponent();
		
		void tick(float) override;
	protected:
		META(Enable);
		std::string Type{LightType::directional};
		META(Enable);
		Vector3 Color{Vector3::zero};
		META(Enable);
		float Intensity{ 0.f };
	};
}