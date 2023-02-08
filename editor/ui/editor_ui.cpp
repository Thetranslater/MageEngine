#include"imgui-docking/imgui.h"
#include"imgui-docking/imgui_internal.h"

#include"ui/editor_ui.h"
#include"context/editor_global_context.h"

#include"engine_core/render_engine/render_system.h"


namespace Mage {
	void EditorUI::drawUI() {
		drawMenuUI();
		drawHierachyUI();
		drawDisplayUI();
		drawInspectorUI();
		drawFileContentUI();
	}
	//TODO:
	void EditorUI::drawMenuUI() {
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground;

		const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(main_viewport->WorkPos);
		ImGui::SetNextWindowSize(main_viewport->WorkSize);
		ImGui::SetNextWindowViewport(main_viewport->ID);

		ImGui::Begin("Menu", nullptr, window_flags);

		//default layout
		ImGuiID main_id = ImGui::GetID("Main Space");
		ImGui::DockSpace(main_id);

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				//file creation
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Asset")) {
				//asset creation
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}
	//TODO:
	void EditorUI::drawHierachyUI() {}
	//TODO:
	void EditorUI::drawDisplayUI() {
		ImGuiWindowFlags display_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;

		if (!ImGui::Begin("DisplayWindow", nullptr, display_flags)) {
			ImGui::End();
			return;
		}

		//TODO:menu

		ImVec2 display_position = ImGui::GetWindowPos();
		ImVec2 display_size		= ImGui::GetWindowSize();

		auto ui_pending = editor_global_context.m_render_system.lock()->getPendingData();
		ui_pending->m_editor.viewport_x = display_position.x;
		ui_pending->m_editor.viewport_y = display_position.y;
		ui_pending->m_editor.viewport_width  = display_size.x;
		ui_pending->m_editor.viewport_height = display_size.y;

		ui_pending->m_camera.m_pending_aspect = display_size.x / display_size.y;

		ImGui::End();
	}
	//TODO:
	void EditorUI::drawInspectorUI() {}
	//TODO:
	void EditorUI::drawFileContentUI() {}
}