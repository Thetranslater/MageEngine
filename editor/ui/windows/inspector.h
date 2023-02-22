#pragma once

#include<ui/windows/window.h>

namespace Mage {
	class GameObject;
	class Inspector :public Window {
	public:
		//hierachy window need to create first before create inspector 
		Inspector(const std::string& id);
		Inspector(const std::string& id, const WindowConfig& config);


		void OnSelected(GameObject&);

		void draw() override;
	private:

	};
}