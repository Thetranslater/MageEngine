#pragma once

#include<ui/widget.h>
#include<ui/container.h>

#include<engine_core/event/event.h>

namespace Mage {

	class TreeNode : public Widget , public Container{
	public:
		TreeNode() = default;
		TreeNode(const std::string& l) :label(l) {}

		virtual void draw() override;
	protected:
		std::string label;
	public:
		bool is_leaf{ false };
		bool is_default_open{ false };
		bool is_open_on_arrow{ false };
		bool is_open_on_double_click{ false };
		bool is_selected{ false };

		Event<> clicked_event;
	};
}