#pragma once

#include<engine_core/render_engine/render_guid.h>
#include<engine_core/render_engine/render_resource.h>

namespace Mage {
	//TODO:model ������һ����Ⱦʵ�壬��GUID������mesh��material����
	struct VkRenderModel {
		GUID32 m_mesh_guid32;
		GUID32 m_material_guid32;
	};
}