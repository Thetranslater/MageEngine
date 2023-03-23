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
		LightComponent() = default;
		
		void tick(float) override;
		const std::string& Type() const;
		const Vector3& Color() const;
		const float Intensity() const;

		void SetType(const std::string&);
		void SetColor(const Vector3&);
		void SetIntensity(float);

		std::shared_ptr<Widget> Draw() override;
	protected:
		META(Enable)
		std::string type{LightType::directional};
		META(Enable)
		Vector3 color{Vector3::zero};
		META(Enable)
		float intensity{ 0.f };
	};
}