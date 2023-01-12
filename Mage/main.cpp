#include<iostream>
//#include<memory>
//#include<string>
//#include<vector>
//#include<sstream>
//#include<fstream>

#include<editor.h>
#include<engine_core/engine.h>

#include"engine_core/render_engine/render_camera.h"
#include"core/math/quaternion.h"

int main() {
	Mage::Vector3 axis_1{ 0.3213f,2.22f,2.f };
	Mage::Vector3 axis_2{ -2.11f,-1.f,0.22f };
	axis_1.Normalize();
	axis_2.Normalize();
	Mage::Quaternion q1 = Mage::Quaternion::AngleAxis(-12.98f, axis_1);
	Mage::Quaternion q2 = Mage::Quaternion::AngleAxis(31.22f, axis_2);

	Mage::Vector3 res_1 = (q2 * q1) * Mage::Vector3{ -1.2f,0.2f,1.f };
	Mage::Vector3 res_2 = q2 * (q1 * Mage::Vector3{ -1.2f,0.2f,1.f });

	Mage::MageEditor* editor = new Mage::MageEditor;
	Mage::MageEngine* engine = new Mage::MageEngine;
	engine->startEngine();
	editor->initialize(engine);

	editor->run();

	editor->shutdown();

	return 0;
}