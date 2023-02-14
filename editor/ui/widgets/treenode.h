#pragma once

#include<ui/widget.h>
#include<ui/container.h>

namespace Mage {

	class TreeNode : public Widget , public Container{
	public:
		TreeNode() = default;
		TreeNode(const std::string& l) :label(l) {}

		virtual void draw() override;
	protected:
		std::string label;
	};
}