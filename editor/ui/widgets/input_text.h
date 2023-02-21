#pragma once

#include<functional>
#include<string>

#include<imgui-docking/imgui.h>

#include<ui/widget.h>

#include<engine_core/event/event.h>

namespace Mage {

	class InputText : public Widget {
	public:
		InputText() :data{ 64 }, lable_id{"##" + std::to_string(id)} {}
		InputText(const std::string& l) : data{64}, lable_id{ l  + "##" + std::to_string(id)} {}

		void draw() override;

		void setContent(const std::string& content);

		void setResizeCallback(const std::function<int(ImGuiInputTextCallbackData*)>& func) { resize = func; }
		void setFilterCallback(const std::function<int(ImGuiInputTextCallbackData*)>& func) { filter = func; }
	private:
		int _callback(ImGuiInputTextCallbackData* data);

		std::string lable_id;
		std::vector<char> data;
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