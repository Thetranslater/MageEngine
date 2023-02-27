#include<iostream>

#define TEST 0

#if TEST == 0

#include<editor.h>
#include<engine_core/engine.h>

#else

#include<core/math/vector3.h>
#include<core/math/quaternion.h>

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
	Vector3 from{ 1,1,1 };
	Vector3 to{ -1,2,-1 };
	Quaternion q1 = Quaternion::FromToRotation(from, to);
	from = { 0,1,1 };
	to = { 2,-1,-3 };
	Quaternion q2 = Quaternion::FromToRotation(from, to);
	Quaternion local = Quaternion::Inverse(q1) * q2;
	local.Normalize();

	Vector3 test{ 1,2,3 };

	Vector3 res1 = q2 * test;
	Vector3 res2 = q1 * test;

	Vector3 axis;
	float angle;
	local.ToAngleAxis(angle, axis);
	axis = q1 * axis;
	local = Quaternion::AngleAxis(angle, axis);
	res2 = local * res2;
#endif //!TEST

	return 0;
}