#include<iostream>
#include<numeric>

#define TEST 0
//#define CONSTRUCTION

#if TEST == 0

#include<editor.h>
#include<engine_core/engine.h>

#else
#include"core/meta/reflection/reflection_register.h"

#include"asset/scene_asset.h"
#include"asset/resource_manager/resource_manager.h"

#include"engine_core/scaffold/components/transformcomponent/Transform_component.h"
#include"engine_core/scaffold/components/meshcomponent/Mesh_component.h"
#include"engine_core/scaffold/components/lightcomponent/Light_component.h"


#endif

 int main() {
#if TEST == 0
	Mage::MageEditor* editor = new Mage::MageEditor;
	Mage::MageEngine* engine = new Mage::MageEngine;
	engine->startEngine();
	editor->initialize(engine);

	editor->run();

	editor->shutdown();
#else 

#ifdef CONSTRUCTION
	 using namespace Mage;
	 Reflection::TypeMetaRegister::Register();

	 SceneAsset defaultScene;
	 defaultScene.name = "Default";

	 std::vector<ObjectAsset> objects(3);
	 {
		 //markov
		 objects[0].name = "Markov";
		 auto markovTransform = MAGE_REFLECTION_NEW(TransformComponent);
		 auto markovMesh = MAGE_REFLECTION_NEW(MeshComponent);

		 auto& meshAsset = markovMesh->Asset();
		 meshAsset.gltf_model_url = "E:/Download/makarov_pistol/scene.gltf";

		 objects[0].components.resize(2);
		 objects[0].components[0] = std::move(markovTransform);
		 objects[0].components[1] = std::move(markovMesh);

		 //floor
		 objects[1].name = "StoneGround";
		 auto stoneTransform = MAGE_REFLECTION_NEW(TransformComponent);
		 auto stoneMesh = MAGE_REFLECTION_NEW(MeshComponent);

		 auto& stoneMeshAsset = stoneMesh->Asset();
		 stoneMeshAsset.gltf_model_url = "E:/Download/stone_floor/scene.gltf";

		 objects[1].components.resize(2);
		 objects[1].components[0] = std::move(stoneTransform);
		 objects[1].components[1] = std::move(stoneMesh);

		 //directional light
		 objects[2].name = "DirectionalLight";
		 auto directionalLightTransform = MAGE_REFLECTION_NEW(TransformComponent);
		 auto directionalLightLight = MAGE_REFLECTION_NEW(LightComponent);

		 objects[2].components.resize(2);
		 objects[2].components[0] = std::move(directionalLightTransform);
		 objects[2].components[1] = std::move(directionalLightLight);
	 }
	 defaultScene.objects = std::move(objects);
	 defaultScene.nodes.resize(3);
	 defaultScene.roots.resize(3);
	 std::iota(defaultScene.roots.begin(), defaultScene.roots.end(), 0);

	 ResourceManager manager;
	 manager.saveAsset("E:/Mage/engine/asset/data/scene/default_scene2.json", defaultScene,nullptr, nullptr);

	 Reflection::TypeMetaRegister::Unregister();
	 
#endif

#endif //!TEST

	return 0;
}