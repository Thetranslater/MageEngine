#include"imgui-docking/imgui.h"
#include"imgui-docking/imgui_internal.h"

#include"ui/editor_ui.h"
#include"ui/windows/dock.h"
#include"ui/windows/hierachy.h"
#include"ui/windows/viewer.h"
#include"ui/windows/inspector.h"
#include"context/editor_global_context.h"

#include"engine_core/render_engine/render_system.h"


namespace Mage {
	void EditorUI::initialize() {
		WindowConfig dock_config{};
		dock_config.no_background = true;
		dock_config.no_move = true;
		dock_config.no_resize = true;
		dock_config.no_close = true;
		dock_config.no_bring_to_front_on_focus = true;
		dock_config.is_dockspace = true;
		dock_config.no_dock = true;
		dock_config.no_titlebar = true;
		dock_config.no_scroll = true;
		dock_config.no_collapse = true;
		dock_config.has_menubar = true;
		dock_window = window_manager.createWindow<Dock>("Dock", dock_config);

		//hierachy
		WindowConfig hierachy_config{};
		hierachy_config.no_collapse = true;
		std::shared_ptr<Hierachy> hierachy_window = window_manager.createWindow<Hierachy>("Hierachy", hierachy_config);
		dock_window->addWindow(hierachy_window);

		//viewer
		WindowConfig viewer_config{};
		viewer_config.no_background = true;
		viewer_config.has_menubar = true;
		viewer_config.no_collapse = true;
		viewer_config.no_scroll = true;
		std::shared_ptr<Viewer> viewer_window = window_manager.createWindow<Viewer>("Viewer", viewer_config);
		dock_window->addWindow(viewer_window);

		//inspector
		WindowConfig inspector_config{};
		inspector_config.no_collapse = true;
		std::shared_ptr<Inspector> inspector_window = window_manager.createWindow<Inspector>("Inspector", inspector_config);
		hierachy_window->on_selected_event.subscribe(std::bind(&Inspector::OnSelected, inspector_window.get(), std::placeholders::_1));
		dock_window->addWindow(inspector_window);

	}

	void EditorUI::drawUI() {
		dock_window->draw();
		//drawMenuUI();
		//drawHierachyUI();
		//drawDisplayUI();
		//drawInspectorUI();
		//drawFileContentUI();
	}
	//TODO:
	void EditorUI::drawMenuUI() {
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus;

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
	void EditorUI::drawHierachyUI() {
		ImGuiWindowFlags hierachy_flags = ImGuiWindowFlags_NoCollapse;

		if (!ImGui::Begin("HierachyWindow", nullptr, hierachy_flags)) {
			ImGui::End();
			return;
		}

		ImGui::BeginGroup();

		for (int i{ 0 }; i < 10; ++i) {
			if (ImGui::TreeNode((void*)(intptr_t)i, "Game Object %d", i)) {
				ImGui::TreePop();
			}
		}

		ImGui::EndGroup();

		ImGui::End();
	}
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
	void EditorUI::drawInspectorUI() {
		ImGuiWindowFlags inspector_flags = ImGuiWindowFlags_NoCollapse;

		if (!ImGui::Begin("InspectorWindow", nullptr, inspector_flags)) {
			ImGui::End();
			return;
		}

		ImGui::End();
	}
	//TODO:
	void EditorUI::drawFileContentUI() {
		ImGuiWindowFlags file_content_flags = ImGuiWindowFlags_NoCollapse;

		if (!ImGui::Begin("FileContentWindow", nullptr, file_content_flags)) {
			ImGui::End();
			return;
		}

		ImGui::End();
	}
}