#pragma once

#include<engine_core/function/global_context/global_context.h>

namespace Mage {
	struct VkRenderModelInfo;

	namespace Util {
		VkRenderModelInfo GetRenderModelInfoFromComponent(ComponentHandle<MeshComponent>& handle);
	}
}