#include<iostream>

//#include<engine_core/engine.h>
#include<asset/resource_manager/resource_manager.h>
#include<asset/resource_manager/asset_type.h>
int main() {
	/*Mage::MageEngine* engine = new Mage::MageEngine();
	engine->startEngine();
	engine->tick(0.f);
	engine->shutdownEngine();*/
	Mage::Buffer markov;
	Mage::ResourceManager loader;
	loader.loadAsset("E:/Download/makarov_pistol/scene.bin", &markov, nullptr, nullptr, true);
	return 0;
}