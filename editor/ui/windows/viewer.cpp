#include"ui/windows/viewer.h"
#include"context/editor_global_context.h"

#include"engine_core/render_engine/render_system.h"
#include"engine_core/render_engine/render_resource.h"

namespace Mage {

	void Viewer::draw() {
		ImGuiWindowFlags flags = _convert_config_to_imgui_();

		if (!ImGui::Begin(window_id.c_str(), configuration.no_close ? nullptr : &is_close, flags)) {
			ImGui::End();
			return;
		}

		ImVec2 p = ImGui::GetWindowPos();
		position = { p.x,p.y };
		ImVec2 s = ImGui::GetWindowSize();
		size = { s.x,s.y };

		ImGui::End();

		auto ui_pending = editor_global_context.m_render_system.lock()->getPendingData();
		ui_pending->m_editor.viewport_x = p.x;
		ui_pending->m_editor.viewport_y = p.y;
		ui_pending->m_editor.viewport_width = s.x;
		ui_pending->m_editor.viewport_height = s.y;
		ui_pending->m_camera.m_pending_aspect = s.x / s.y;
	}
}