#include"ui/windows/viewer.h"
#include"context/editor_global_context.h"

#include"engine_core/render_system/render_pending_data.h"
#include"engine_core/render_system/render_system.h"
#include"engine_core/render_system/render_resource.h"

namespace Mage {

	void Viewer::draw() {
		ImGuiWindowFlags flags = _convert_config_to_imgui();

		if (!ImGui::Begin(window_id.c_str(), configuration.no_close ? nullptr : &is_close, flags)) {
			ImGui::End();
			return;
		}

		_update();

		ImGui::End();

		auto ui_pending = editor_global_context.m_render_system.lock()->getPendingData();
		ui_pending->m_editor.viewport_x = position.x;
		ui_pending->m_editor.viewport_y = position.y;
		ui_pending->m_editor.viewport_width = size.x;
		ui_pending->m_editor.viewport_height = size.y;
		ui_pending->m_camera.m_pending_aspect = size.x / size.y;
	}
}