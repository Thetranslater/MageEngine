#pragma once
#include<memory>
#include<string>

#include"engine_core/scaffold/components/component.h"
#include"engine_core/scaffold/components/meshcomponent/mesh_asset.h"

namespace Mage {
	class MeshAsset;
	REFLECTION_TYPE(MeshComponent)
	CLASS(MeshComponent : public Component, WhiteListFields) {
		REFLECTION_BODY(MeshComponent)
	public:
		MeshComponent() { should_tick_in_editor = true; }
		//void tick(float delta) override;

		std::shared_ptr<Widget> Draw() override;
		bool IsLoad() const { return is_loaded; }
		void SetLoad() { is_loaded = true; }
		MeshAsset& Asset() { return mesh; }
	private:
		META(Enable)
		MeshAsset mesh;

		bool is_loaded{ false };
	};
}