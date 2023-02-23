#include"imgui-docking/imgui.h"
#include"imgui-docking/imgui_internal.h"

#include"core/meta/reflection/reflection.h"

#include"ui/editor_ui.h"
#include"ui/windows/dock.h"
#include"ui/windows/hierachy.h"
#include"ui/windows/viewer.h"
#include"ui/windows/inspector.h"
#include"ui/widgets/widget.h"
#include"ui/widgets/container.h"
#include"ui/widgets/sameline.h"
#include"ui/widgets/treenode.h"
#include"ui/widgets/group.h"
#include"ui/widgets/drag_float.h"
#include"ui/widgets/text.h"
#include"ui./widgets/checkbox.h"
#include"ui/widgets/WFI/bindable.h"
#include"context/editor_global_context.h"

#include"engine_core/render_engine/render_system.h"


namespace Mage {
	std::unordered_map<std::string, EditorUI::CreateFunc> EditorUI::base_widget_creator;

	void EditorUI::initialize() {
		//base widget creator initialize
		base_widget_creator["TreeNode"] = [&](const std::string& lable, void* instance) -> std::shared_ptr<Widget> {
			auto treenode = CREATE_WIDGET(TreeNode, lable);
			treenode->is_open_on_arrow = true;
			treenode->is_open_on_double_click = true;

			Reflection::TypeMeta meta{ Reflection::TypeMeta::newMetaFromName(lable) };
			Reflection::FieldAccessor* fields;
			int field_count = meta.getFieldsList(fields);
			for (int i{ 0 }; i < field_count; ++i) {
				if (base_widget_creator.find(fields[i].getFieldTypeName()) != base_widget_creator.end()) {
					treenode->addWidget(
						base_widget_creator[fields[i].getFieldTypeName()](
							fields[i].getFieldName(),
							fields[i].get(instance)));
				}
				else {
					treenode->addWidget(
						base_widget_creator["TreeNode"](
							fields[i].getFieldTypeName(),
							fields[i].get(instance)));
				}
			}
			delete[] fields;

			return treenode;
		};
		base_widget_creator["Vector3"] = [&](const std::string& lable, void* instance) -> std::shared_ptr<Widget> {
			auto group = CREATE_WIDGET(Group);

			group->addWidget(CREATE_WIDGET(Text, lable));
			group->addWidget(CREATE_WIDGET(SameLine));
			Reflection::TypeMeta meta{ Reflection::TypeMeta::newMetaFromName("Vector3")};
			Reflection::FieldAccessor* fields{ nullptr };
			int field_count = meta.getFieldsList(fields);
			ID last_id{ invalid_widget_id };
			for (int i{ 0 }; i < field_count; ++i) {
				group->addWidget(
					base_widget_creator["float"](
						fields[i].getFieldName(),  
						fields[i].get(instance)));

				last_id = group->addWidget(CREATE_WIDGET(SameLine));
			}
			group->removeWidget(last_id);

			delete[] fields;

			return group;
		};
		//TODO:不知道对不对
		base_widget_creator["Quaternion"] = [&](const std::string& lable, void* instance) -> std::shared_ptr<Widget>{
			auto group = CREATE_WIDGET(Group, CREATE_WIDGET(SameLine), CREATE_WIDGET(Text, lable));
			ID last_id{ invalid_widget_id };

#define QuatFieldBind(field) \
			group->addWidget(CREATE_WIDGET(Text, #field));\
			group->addWidget(CREATE_WIDGET(SameLine));\
			auto drag_##field## = CREATE_WIDGET(DragFloat); \
			drag_##field##->is_always_clamp = true; \
			auto getter_##field## = [](void* in) -> float { \
				float _##field## = static_cast<Quaternion*>(in)->ToEulerAngles().##field##; return _##field##; \
			}; \
			auto setter_##field## = [](void* in, float& value) { \
				Vector3 angle{ static_cast<Quaternion*>(in)->ToEulerAngles() }; \
				angle.##field## = value; \
				static_cast<Quaternion*>(in)->SetEulerAngels(angle); \
			}; \
			auto bind_##field## = CREATE_WFI(Bindable<float>); \
			bind_##field##->bind(std::bind(getter_##field##, instance), std::bind(setter_##field##, instance, std::placeholders::_1)); \
			drag_##field##->addWFI(bind_##field##); \
			group->addWidget(drag_##field##);\
			last_id = group->addWidget(CREATE_WIDGET(SameLine));
			

			QuatFieldBind(x);
			QuatFieldBind(y);
			QuatFieldBind(z);
#undef QuatFieldBind

			group->removeWidget(last_id);

			return group;
		};
		base_widget_creator["float"] = [&](const std::string& lable, void* instance) -> std::shared_ptr<Widget>{
			auto group = CREATE_WIDGET(Group);
			group->addWidget(CREATE_WIDGET(Text, lable));
			group->addWidget(CREATE_WIDGET(SameLine));
			
			auto drag = CREATE_WIDGET(DragFloat);
			drag->is_always_clamp = true;
			auto getter = [](void* in) -> float { return *static_cast<float*>(in); };
			auto setter = [](void* in, float& value) {*static_cast<float*>(in) = value; };
			auto bind_wfi = CREATE_WFI(Bindable<float>);
			bind_wfi->bind(std::bind(getter, instance), std::bind(setter, instance, std::placeholders::_1));
			drag->addWFI(bind_wfi);

			group->addWidget(drag);

			return group;
		};
		base_widget_creator["bool"] = [&](const std::string& lable, void* instance) -> std::shared_ptr<Widget> {
			auto group = CREATE_WIDGET(Group, CREATE_WIDGET(SameLine), CREATE_WIDGET(Text, lable));

			auto checkbox = CREATE_WIDGET(CheckBox);
			auto getter = [](void* instance) -> bool {return *static_cast<bool*>(instance); };
			auto setter = [](void* instance, bool& val) {*static_cast<bool*>(instance) = val; };
			auto bind_wfi = CREATE_WFI(Bindable<bool>);
			bind_wfi->bind(std::bind(getter, instance), std::bind(setter, instance, std::placeholders::_1));
			checkbox->addWFI(bind_wfi);
			group->addWidget(checkbox);

			return group;
		};
		base_widget_creator["int"] = [&](const std::string& lable, void* instance) -> std::shared_ptr<Widget> {

		};

		WindowConfig dock_config{};
		dock_config.no_background = true;
		dock_config.no_move = true; 
		dock_config.no_resize = true;
		dock_config.no_close = true;
		dock_config.no_bring_to_front_on_focus = true;
		dock_config.is_dockspace = true;
		dock_config.no_dock = true;
		dock_config.no_titlebar = true;
		dock_config.no_scroll = true;
		dock_config.no_collapse = true;
		dock_config.has_menubar = true;
		dock_window = window_manager.createWindow<Dock>("Dock", dock_config);

		//hierachy
		WindowConfig hierachy_config{};
		hierachy_config.no_collapse = true;
		std::shared_ptr<Hierachy> hierachy_window = window_manager.createWindow<Hierachy>("Hierachy", hierachy_config);
		dock_window->addWindow(hierachy_window);

		//viewer
		WindowConfig viewer_config{};
		viewer_config.no_background = true;
		viewer_config.has_menubar = true;
		viewer_config.no_collapse = true;
		viewer_config.no_scroll = true;
		std::shared_ptr<Viewer> viewer_window = window_manager.createWindow<Viewer>("Viewer", viewer_config);
		dock_window->addWindow(viewer_window);

		//inspector
		WindowConfig inspector_config{};
		inspector_config.no_collapse = true;
		std::shared_ptr<Inspector> inspector_window = window_manager.createWindow<Inspector>("Inspector", inspector_config);
		hierachy_window->on_selected_event.subscribe(std::bind(&Inspector::OnSelected, inspector_window.get(), std::placeholders::_1));
		dock_window->addWindow(inspector_window);

	}

	void EditorUI::drawUI() {
		dock_window->draw();
	}
	//TODO:
	void EditorUI::drawFileContentUI() {
		ImGuiWindowFlags file_content_flags = ImGuiWindowFlags_NoCollapse;

		if (!ImGui::Begin("FileContentWindow", nullptr, file_content_flags)) {
			ImGui::End();
			return;
		}

		ImGui::End();
	}
}