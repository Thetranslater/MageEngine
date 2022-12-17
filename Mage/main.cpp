#include<iostream>

#include<engine_core/engine.h>

int main() {
	Mage::MageEngine* engine = new Mage::MageEngine;
	engine->startEngine();
	engine->tick(1.f);
	engine->shutdownEngine();
	return 0;
}