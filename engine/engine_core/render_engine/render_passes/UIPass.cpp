#include"imgui-docking/imgui.h"
#include"imgui-docking/backends/imgui_impl_vulkan.h"
#include"imgui-docking/backends/imgui_impl_glfw.h"

#include"ui/editor_ui.h"

#include"engine_core/render_engine/renderer/vulkanRHI.h"
#include"engine_core/render_engine/render_passes/UIPass.h"

namespace Mage {
	void UISubpass::initialize(SubpassCreateInfo* create_info) {
		Subpass::initialize(create_info);

		UISubpassCreateInfo* ui_info = static_cast<UISubpassCreateInfo*>(create_info);

		m_editor_ui = ui_info->info_editor_ui;
	}

	void UISubpass::draw() {
		if (m_editor_ui != nullptr) {
			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			m_editor_ui->drawUI();

			ImGui::Render();

			ImDrawData* data = ImGui::GetDrawData();

			ImGui_ImplVulkan_RenderDrawData(data, m_vulkan_rhi->getCurrentCommandBuffer());
		}
	}
}