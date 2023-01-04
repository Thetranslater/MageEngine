#include<iostream>
#include<memory>
#include<exception>

//#include<engine_core/engine.h>
#include"core/_generated/serializer/all_serializer.h"
#include"core/meta/reflection/reflection.h"
#include"core/meta/serializer/serializer.h"
//#include"TEST.h"

int main() {
	Mage::WorldAsset temp;
	temp.name = "3121321";
	temp.scene_urls.resize(3);
	temp.scene_urls[0] = "ab";
	temp.scene_urls[1] = "bc";
	temp.scene_urls[2] = "cd";
	auto res = Mage::PSerializer::write(temp);
	std::string str = res.dump();

	//Mage::MageEngine* engine = new Mage::MageEngine();
	//engine->startEngine();
	//engine->run(1.f);
	//engine->shutdownEngine();
	return 0;
}