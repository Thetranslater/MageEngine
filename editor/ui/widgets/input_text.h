#pragma once

#include<functional>
#include<string>

#include<imgui-docking/imgui.h>

#include<ui/widgets/widget.h>
#include<ui/widgets/WFI/bindable.h>

#include<engine_core/event/event.h>

namespace Mage {
	//TODO:感觉会有问题，当输入大于当前string的size时会重新分配内存，到时候会发生什么，我不知道（我似乎应该给callback函数一个默认实现，以免出现意外情况）
	class InputText : public DataWidget<std::string> {
	public:
		InputText() { value.resize(64); }
		InputText(const std::string& l) : lable{l} { value.resize(64); }

		void draw() override;

		void setContent(const std::string& content);

		void setResizeCallback(const std::function<int(ImGuiInputTextCallbackData*)>& func) { resize = func; has_callback_resize = true; }
		void setFilterCallback(const std::function<int(ImGuiInputTextCallbackData*)>& func) { filter = func; has_callback_filter = true; }
	private:
		int _callback(ImGuiInputTextCallbackData* data);
		//TODO:default callbacks implementation
		int _default_resize(ImGuiInputTextCallbackData* data);
		int _default_filter(ImGuiInputTextCallbackData* data);

		std::string lable;

		std::function<int(ImGuiInputTextCallbackData*)> resize;
		std::function<int(ImGuiInputTextCallbackData*)> filter;
	public:
		bool is_decimal{ false };
		bool is_hexadecimal{ false };
		bool is_noblank{ false };
		bool is_uppercase{ false };
		bool is_readonly{ false };
		bool is_password{ false };
		bool has_callback_resize{ false };
		bool has_callback_filter{ false };

		Event<std::string> changed_event;
	};
}