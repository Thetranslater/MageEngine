#pragma once

#include<engine_core/render_engine/render_guid.h>
#include<engine_core/render_engine/render_resource.h>

namespace Mage {
	//TODO:model 代表着一个渲染实体，用GUID索引其mesh和material数据
	struct VkRenderModel {
		GUID32 m_mesh_guid32;
		GUID32 m_material_guid32;
	};
}