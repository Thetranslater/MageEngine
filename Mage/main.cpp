#include<iostream>
#include<memory>
#include<exception>

#include<engine_core/engine.h>

int main() {

	Mage::MageEngine* engine = new Mage::MageEngine();
	engine->startEngine();
	engine->run(1.f);
	engine->shutdownEngine();
	return 0;
}