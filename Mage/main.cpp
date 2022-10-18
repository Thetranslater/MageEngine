#include<asset/resource_manager/resource_manager.h>

#include<iostream>
int main() {
	Mage::ResourceManager loader;
	tinygltf::Model model;
	if (!loader.loadModelFromFile("E:/Download/makarov_pistol/scene.gltf", &model, nullptr, nullptr)) {
		std::cout << false << std::endl;
	}
	return 0;
}