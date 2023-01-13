#include<iostream>

#include<editor.h>
#include<engine_core/engine.h>

//#include"engine_core/render_engine/render_camera.h"
//#include"core/math/quaternion.h"

//struct Transform {
//	Mage::Vector3 X{ 0.f,1.f,0.f };
//	Mage::Vector3 Y{ 0.f,0.f,1.f };
//	Mage::Vector3 Z{ 1.f,0.f,0.f };
//
//	Mage::Vector3 position{ 0.f,0.f,0.f };
//
//	Transform() = default;
//
//	void rotate(const Mage::Quaternion& q) {
//		X = q * X;
//		Y = q * Y;
//		Z = q * Z;
//	}
//
//	void move(const Mage::Vector3& v) {
//		position += v;
//	}
//
//	Mage::Matrix4x4 viewMatrix() {
//		Mage::Matrix4x4 view;
//		view[0][0] = X.x;
//		view[0][1] = X.y;
//		view[0][2] = X.z;
//		view[1][0] = Y.x;
//		view[1][1] = Y.y;
//		view[1][2] = Y.z;
//		view[2][0] = Z.x;
//		view[2][1] = Z.y;
//		view[2][2] = Z.z;
//		view[3][0] = 0;
//		view[3][1] = 0;
//		view[3][2] = 0;
//		view[0][3] = -Mage::Vector3::Dot(X, position);
//		view[1][3] = -Mage::Vector3::Dot(Y, position);
//		view[2][3] = -Mage::Vector3::Dot(Z, position);
//		view[3][3] = 1;
//		return view;
//	}
//};

int main() {
	//Mage::Quaternion rotation{ Mage::Quaternion::identity };
	//rotation = Mage::Quaternion::AngleAxis(90.f, Mage::Vector3{ 1.f,0.f,0.f });
	//rotation = rotation * Mage::Quaternion::AngleAxis(45.f, Mage::Vector3{ 0.f,1.f,0.f });

	//Transform camera;
	////camera.move(Mage::Vector3{ 2.f,1.f,-2.f });
	//camera.rotate(rotation);
	////camera.rotate(Mage::Quaternion::AngleAxis(45.f, Mage::Vector3{ 0.f,1.f,0.f }));

	//Mage::Matrix4x4 view = camera.viewMatrix();

	//Mage::Vector3 world_point{ 0.f,1.f,0.f };

	//auto local_point = view * world_point;

	Mage::MageEditor* editor = new Mage::MageEditor;
	Mage::MageEngine* engine = new Mage::MageEngine;
	engine->startEngine();
	editor->initialize(engine);

	editor->run();

	editor->shutdown();

	return 0;
}