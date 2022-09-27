//#include<engine_core/engine.h>
#include<asset/resource_manager/resource_manager.h>

int main() {
	Mage::ResourceManager loader;
	tinygltf::Model markov;
	std::string err;
	std::string warn;
	if (!loader.loadModelFromFile("E:\\Download\\makarov_pistol\\scene.gltf", &markov, &err, &warn)) {
		throw std::runtime_error("failed to load markov model");
	}

	/*Mage::MageEngine* engine = new Mage::MageEngine();
	engine->startEngine();
	while (true) {
		engine->tick(0.f);
	}*/
	return 0;
}