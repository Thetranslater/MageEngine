#pragma once
#include<imgui-docking/imgui.h>

#include"core/math/vector2.h"

#include"ui/container.h"

namespace Mage {
	struct WindowConfig {
		bool no_resize = false;
		bool no_move = false;
		bool no_close = false;
		bool no_scroll = false;
		bool no_dock = false;
		bool no_background = false;
		bool no_inputs = false;
		bool no_collapse = false;
		bool no_titlebar = false;
		bool no_bring_to_front_on_focus = false;

		bool is_dockspace = false;
		//TODO:...
		bool has_menubar = false;
	};

	class Window : public Container {
	public:
		Window(const std::string& name) :window_id{ name } {}
		Window(const std::string& name, const WindowConfig& config) :window_id{ name }, configuration{ config } {}

		inline Vector2 getWindowPos() const { return position; }
		inline Vector2 getWindowSize() const { return size; }

		inline const std::string& getID() { return window_id; }

		virtual void draw() {};
	protected:
		inline ImGuiWindowFlags _convert_config_to_imgui() const {
			ImGuiWindowFlags flags = ImGuiWindowFlags_None;

			if (configuration.no_resize)					flags |= ImGuiWindowFlags_NoResize;
			if (configuration.no_move)						flags |= ImGuiWindowFlags_NoMove;
			if (configuration.no_scroll)					flags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar;
			if (configuration.no_dock)						flags |= ImGuiWindowFlags_NoDocking;
			if (configuration.no_titlebar)					flags |= ImGuiWindowFlags_NoTitleBar;
			if (configuration.no_background)				flags |= ImGuiWindowFlags_NoBackground;
			if (configuration.no_inputs)					flags |= ImGuiWindowFlags_NoInputs;
			if (configuration.no_collapse)					flags |= ImGuiWindowFlags_NoCollapse;
			if (configuration.no_bring_to_front_on_focus)	flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			if (configuration.has_menubar)					flags |= ImGuiWindowFlags_MenuBar;

			return flags;
		}

		//TODO
		inline void _update() {
			ImVec2 p = ImGui::GetWindowPos();
			position = { p.x,p.y };
			ImVec2 s = ImGui::GetWindowSize();
			size = { s.x,s.y };
		}

		WindowConfig configuration{};

		//state
		Vector2 position;
		Vector2 size;

		bool is_close;

		std::string window_id;
	};
}