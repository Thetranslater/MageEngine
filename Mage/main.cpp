#include<iostream>
#include<memory>
#include<string>
#include<vector>
#include<sstream>
#include<fstream>

#include<engine_core/engine.h>

int main() {
	Mage::MageEngine* engine = new Mage::MageEngine();
	engine->startEngine();
	engine->run();
	engine->shutdownEngine();
	return 0;
}