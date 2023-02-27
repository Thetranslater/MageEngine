#pragma once

#include<core/meta/reflection/reflection.h>

namespace Mage {
	struct LightType {
		static const std::string directional;
		static const std::string point;
		//TODO: spot and area light
	};

	inline const std::string LightType::directional = "Directional";
	inline const std::string LightType::point = "Point";
}