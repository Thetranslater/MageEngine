#pragma once

#include<engine_core/render_system/render_pass.h>
#include<engine_core/render_system/render_subpass.h>

#define DIRECTIONAL_SHADOW_MAP_DIMENSION_SIZE 2048u

namespace Mage {
	struct DirectionalShadowPassCreateInfo : public RenderPassCreateInfo {};

	class DirectionalShadowPass : public RenderPass {
	public:
		void initialize(const RenderPassCreateInfo*) override final;
		void draw() override final;
	private:
		void setupAttachments();
		void setupRenderPass();
		void setupDescriptorLayouts();
		void setupDescriptorSets();
		void setupSubPasses();
		void setupFramebuffers();

		const uint32_t m_subpass_count = 1;
	};

	struct DirectionalShadowSubpassCreateInfo : public SubpassCreateInfo {
		std::vector<int> info_layout_indices;
	};

	class DirectionalShadowSubpass : public Subpass {
	public:
		void initialize(SubpassCreateInfo* info) override final;

		void draw() override final;
	private:
		void setupPipeline(const std::vector<int>&) override final;
	};
}