#include"core/macro.h"

#include"ui/widgets/input_text.h"

namespace Mage {
	int InputText::_callback(ImGuiInputTextCallbackData* data) {
		int res{ -1 };
		switch (data->EventFlag)
		{
		case ImGuiInputTextFlags_CallbackCharFilter:
			res = filter(data);
			break;
		case ImGuiInputTextFlags_CallbackResize:
			res = resize(data);
			break;
		default:
			MAGE_THROW(none implement callback)
		}
		return res;
	}

	void InputText::draw() {
		ImGuiInputTextFlags flags{ ImGuiInputTextFlags_None };

		if (is_decimal)				flags |= ImGuiInputTextFlags_CharsDecimal;
		if (is_hexadecimal)			flags |= ImGuiInputTextFlags_CharsHexadecimal;
		if (is_noblank)				flags |= ImGuiInputTextFlags_CharsNoBlank;
		if (is_password)			flags |= ImGuiInputTextFlags_Password;
		if (is_readonly)			flags |= ImGuiInputTextFlags_ReadOnly;
		if (is_uppercase)			flags |= ImGuiInputTextFlags_CharsUppercase;
		if (has_callback_resize)	flags |= ImGuiInputTextFlags_CallbackResize;
		if (has_callback_filter)	flags |= ImGuiInputTextFlags_CallbackCharFilter;

		std::vector<char> pre{ data.begin(),data.end() };

		if (hasBind()) {
			std::copy(get().begin(), get().end(), data.begin());
		}

		if (has_callback_filter or has_callback_resize) {
			//TODO:���callback�ܷ��������ɶ��Ǹ����⣬��û����
			ImGui::InputText(lable_id.c_str(), data.data(), data.size(), flags, *std::function<int(ImGuiInputTextCallbackData*)>{ std::bind(&InputText::_callback, this, std::placeholders::_1) }.target<ImGuiInputTextCallback>());
		}
		else {
			ImGui::InputText(lable_id.c_str(), data.data(), data.size(), flags);
		}

		if (not std::equal(data.begin(), data.end(), pre.begin(), pre.end())) {
			changed_event.invoke(std::string{ data.data() });
		}

		if (hasBind()) {
			std::string str{ data.data() };
			set(str);
		}
	}

	void InputText::setContent(const std::string& content) {
		data.clear();
		data.resize(content.size());
		std::copy(content.begin(), content.end(), data.begin());
		data.emplace_back('\0');
	}
}