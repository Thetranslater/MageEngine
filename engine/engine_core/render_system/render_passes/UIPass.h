#pragma once

#include"engine_core/render_system/render_subpass.h"

namespace Mage {
	class EditorUI;

	struct UISubpassCreateInfo :public SubpassCreateInfo {
		EditorUI* info_editor_ui;
	};

	class UISubpass :public Subpass {
	public:
		void initialize(SubpassCreateInfo*) override final;

		void draw() override final;

		void clean() override final {};
	private:
		void setupPipeline(const std::vector<int>&) override final {};

		EditorUI* m_editor_ui;
	};
}