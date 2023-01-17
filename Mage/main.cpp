#include<iostream>

#include<editor.h>
#include<engine_core/engine.h>

int main() {
	Mage::MageEditor* editor = new Mage::MageEditor;
	Mage::MageEngine* engine = new Mage::MageEngine;
	engine->startEngine();
	editor->initialize(engine);

	editor->run();

	editor->shutdown();

	return 0;
}