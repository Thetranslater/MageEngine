#include<iostream>

#include<asset/resource_manager/asset_type.h>
#include<asset/resource_manager/resource_manager.h>
int main() {
	Mage::Model model;
	Mage::ResourceManager loader;
	if (!loader.loadModelFromFile("E:/Download/makarov_pistol/scene.gltf", &model, nullptr, nullptr)) {
		std::cout << "false" << std::endl;
	}

	return 0;
}