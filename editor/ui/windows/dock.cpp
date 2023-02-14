#include<imgui-docking/imgui.h>

#include"ui/windows/dock.h"

namespace Mage {
	void Dock::draw() {
		ImGuiWindowFlags flags = _convert_config_to_imgui_();

		if (configuration.is_dockspace) {
			ImGuiViewport* viewport_ptr = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport_ptr->WorkPos);
			ImGui::SetNextWindowSize(viewport_ptr->WorkSize);
			ImGui::SetNextWindowViewport(viewport_ptr->ID);
		}

		if (!ImGui::Begin(window_id.c_str(), configuration.no_close ? nullptr : &is_close, flags)) {
			ImGui::End();
			return;
		}

		if (configuration.is_dockspace) {
			ImGuiID dockID = ImGui::GetID(window_id.c_str());
			ImGui::DockSpace(dockID);
		}

		//draw
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

		//TODO: update state
		ImVec2 pos = ImGui::GetWindowPos();
		position[0] = pos.x;
		position[1] = pos.y;

		ImVec2 sz = ImGui::GetWindowSize();
		size[0] = sz.x;
		size[1] = sz.y;

		for (const auto& widget_ptr : widgets) {
			widget_ptr->draw();
		}

		ImGui::End();

		for (const auto& window_ptr : children) {
			window_ptr->draw();
		}
	}
}