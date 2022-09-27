#pragma once
#include"engine_core/render_engine/render_pass.h"
#include"engine_core/render_engine/render_subpass.h"

namespace Mage {
	struct DevRenderPassCreateInfo :public RenderPassCreateInfo {};

	class DevRenderPass :public RenderPass {
	public:
		void initialize(const RenderPassCreateInfo* createInfo) override final;

		void draw() override final;

	private:
		void setupFramebuffers();
		void setupSubpasses();
		void setupDescriptorLayouts();
		void setupDescriptorSets();
		void setupRenderPass();

		//because the renderpass is created before subpass, so you need manually specifies the subpass count.
		uint32_t m_subpass_count{ 1 };
	};
	struct DevRenderSubpassCreateInfo :public SubpassCreateInfo {};
	//！！！！！！！！！！！！！！                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
	class DevRenderSubpass :public Subpass {
	public:
		void initialize(const SubpassCreateInfo* createInfo) override final;

		void draw() override final;
	private:
		void setupDescriptorSetLayouts(const std::vector<int>& indices) override final;
		void setupDescriptorSets(const std::vector<int>& indices) override final;
		void setupPipeline() override final;
	};
}