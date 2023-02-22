#pragma once

#include<vector>
#include<memory>

#include<ui/widgets/widget.h>

namespace Mage {
	class Container {
	public:
		ID addWidget(std::shared_ptr<Widget>&& r_widget_ptr) {
			ID id = r_widget_ptr->getID();
			widgets.emplace_back(std::move(r_widget_ptr));
			return id;
		}

		void removeWidget(const ID widget_id) {
			widgets.erase(std::remove_if(widgets.begin(), widgets.end(), [&widget_id](const std::shared_ptr<Widget>& widget_ptr) {
				return widget_id == widget_ptr->getID();
				}), widgets.end());
		}
		void removeWidget(const Widget& widget) {
			widgets.erase(std::remove_if(widgets.begin(), widgets.end(), [&widget](const std::shared_ptr<Widget>& widget_ptr) {
				return widget_ptr->getID() == widget.getID();
				}), widgets.end());
		}
	protected:
		std::vector<std::shared_ptr<Widget>> widgets;
	};
}