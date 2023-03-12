#pragma once
#include"engine_core/render_system/render_pass.h"
#include"engine_core/render_system/render_subpass.h"

namespace Mage {
	struct ForwardRenderPassCreateInfo : public RenderPassCreateInfo{};

	class ForwardRenderPass :public RenderPass {

		enum SubpassType {
			subpass_type_forward,
			subpass_type_ui
		};

	public:
		void initialize(const RenderPassCreateInfo*) override final;

		void draw() override final;

		void recreateAfterRHI();
	private:
		void setupRenderPass();
		void setupDescriptorLayouts();
		void setupDescriptorSets();
		void setupSubPasses();
		void setupFramebuffers();

		uint32_t m_subupass_count{ 1 };
	};

	struct ForwardRenderSubpassCreateInfo :public SubpassCreateInfo {
		std::vector<int> info_layout_indices;
	};

	class ForwardRenderSubpass :public Subpass {
	public:
		void initialize(SubpassCreateInfo*) override final;

		void draw() override final;

		void clean() override final;
	private:
		void setupPipeline(const std::vector<int>&) override final;
	};
}