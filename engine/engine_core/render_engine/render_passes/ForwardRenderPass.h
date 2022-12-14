#pragma once
#include"engine_core/render_engine/render_pass.h"
#include"engine_core/render_engine/render_subpass.h"

namespace Mage {
	struct ForwardRenderPassCreateInfo:public RenderPassCreateInfo{};

	class ForwardRenderPass :public RenderPass {
	public:
		void initialize(const RenderPassCreateInfo* createInfo) override final;

		void draw() override final;

	private:
		void setupRenderPass();
		void setupDescriptorLayouts();
		void setupDescriptorSets();
		void setupSubPasses(){};
		void setupFramebuffers(){};

		uint32_t m_subupass_count{ 1 };
	};

	struct ForwardRenderSubpassCreateInfo :public SubpassCreateInfo {
		std::vector<int> m_layouts_indices;
	};

	class ForwardRenderSubpass :public Subpass {
	public:
		void initialize(SubpassCreateInfo* createInfo) override final;

		void draw() override final;
	private:
		//void setupDescriptorSetLayouts(const std::vector<int>& indices) override final;
		//void setupDescriptorSets(const std::vector<int>& indices) override final;
		void setupPipeline(const std::vector<int>& indices) override final;
	};
}