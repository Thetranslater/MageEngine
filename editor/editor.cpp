#include"core/macro.h"

#include"imgui-docking/imgui.h"

#include"editor.h"
#include"input/editor_input.h"
#include"context/editor_global_context.h"

#include"engine_core/function/global_context/global_context.h"
#include"engine_core/engine.h"
#include"engine_core/input/input_system.h"
#include"engine_core/render_system/render_system.h"

namespace Mage {
	void MageEditor::initialize(MageEngine* engine) {
		m_engine = engine;

		editor_global_context.initialize(engine_global_context);

		m_engine->postInitialize();

		initializeUI();
	}

	void MageEditor::shutdown() {
		//TODO:editor shutdown
		m_engine->shutdownEngine();
	}

	void MageEditor::run() {
		while (true) {
			//calculate delta time
			float delta_time = 1.f;
			//input tick
			engine_global_context.m_input_system->tick();
			//editor tick
			editor_global_context.m_editor_input->tick(delta_time);
			//engine tick
			if (m_engine->tick(delta_time)) {
				return;
			}
		}
	}

	void MageEditor::initializeUI() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		//set imgui io
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//set imgui style
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 0;
		style.ChildBorderSize = 0;

		style.WindowPadding = ImVec2{ 0,0 };
		style.FramePadding = ImVec2{ 5,4 };
		style.ItemSpacing = ImVec2{ 8,5 };
		style.IndentSpacing = 20;

		style.FrameRounding = 4;

		style.WindowMenuButtonPosition = ImGuiDir_None;

		editor_global_context.m_render_system.lock()->initializeUIBackend();
	}
}