#include<iostream>

#define TEST 0

#if TEST == 0

#include<editor.h>
#include<engine_core/engine.h>

#else

#include"ui/util.h"

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
	 using namespace Mage;

	 std::string out;
	 std::vector<std::pair<std::string, std::string>> types = { std::make_pair("Models", "*.gltf*") };

	 Mage::Util::OpenFileDialog(out, types);
#endif //!TEST

	return 0;
}