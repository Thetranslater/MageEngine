#include"asset/resource_manager/resource_manager.h"
#include"asset/resource_manager/asset_type.h"

#include"engine_core/scaffold/game_object.h"
#include"engine_core/scaffold/components/meshcomponent/Mesh_component.h"
#include"engine_core/render_engine/render_system.h"
#include"engine_core/render_engine/render_scene.h"
#include"engine_core/function/global_context/global_context.h"

#include"ui/util.h"
#include"ui/widgets/treenode.h"
#include"ui/widgets/group.h"
#include"ui/widgets/table.h"
#include"ui/widgets/text.h"
#include"ui/widgets/sameline.h"
#include"ui/widgets/input_text.h"
#include"ui/widgets/button.h"

#include"ui/widgets/WFI/bindable.h"
#include"ui/widgets/WFI/resizable.h"

namespace Mage{
	void MeshComponent::tick(float delta) {
		if (should_tick_in_editor) {
			if (not is_loaded) {
				auto render_scene = engine_global_context.m_render_system->getRenderScene();
				auto loader = engine_global_context.m_resource_manager;
				
				Model mage_model;
				std::string err;
				bool load_res = loader->loadMageModel(mesh.gltf_model_url, &mage_model, &err, nullptr, false);
				//mesh load(fake),将mesh数据填好打包给render scene
				VkRenderModelInfo render_model = mage_model.getVkRenderModelInfo();
				render_model.m_go_id = game_object->getInstanceID();
				render_scene->m_p_scene_load_deque->push_back(render_model);
				is_loaded = true;
			}
		}
	}

	std::shared_ptr<Widget> MeshComponent::Draw() {
		auto node = CREATE_WIDGET(TreeNode, "Mesh");

		auto ui_table = std::make_shared<Table<1,2>>();
		ui_table->is_size_fixed_fit = true;

		ui_table->operator[](0)[0] = CREATE_WIDGET(Text, "GltfAsset");
		{
			auto group = CREATE_WIDGET(Group);

			auto path_bindable = CREATE_WFI(Bindable<std::string>);
			auto getter = [](void* instance) ->std::string {
				return *static_cast<std::string*>(instance);
			};
			auto setter = [](void* instance, std::string& val) {
				*static_cast<std::string*>(instance) = val;
			};
			path_bindable->bind(std::bind(getter, &mesh.gltf_model_url), std::bind(setter, &mesh.gltf_model_url, std::placeholders::_1));
			auto path_resizable = CREATE_WFI(Resizable, 220.f);
			auto input = CREATE_WIDGET(InputText);
			input->addWFI(path_bindable);
			input->addWFI(path_resizable);

			group->addWidget(input);
			group->addWidget(CREATE_WIDGET(SameLine, 0.f, 0.f));

			auto button = CREATE_WIDGET(Button, 15.f, 0.f);
			Util::FileTypeFilters model_file_filter = { MakeFileTypeFilter(Models, .gltf)};
			button->clicked_event.subscribe(std::bind(Util::OpenFileDialog, mesh.gltf_model_url, model_file_filter));
			group->addWidget(button);

			ui_table->operator[](0)[1] = group;
		}
		node->addWidget(ui_table);
		return node;
	}
}