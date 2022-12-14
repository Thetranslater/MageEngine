#include<iostream>

//#include<engine_core/engine.h>
//#include<asset/resource_manager/resource_manager.h>
//#include<asset/resource_manager/asset_type.h>
#include<core/math/matrix4x4.h>
int main() {
	Mage::Matrix4x4 matrix;
	auto perspective = matrix.Perspective(75.f, 1.2f, 1.f, 10.f);
	auto invper = perspective.inverse();
	Mage::Vector4 a{ 1,1,1,1 };
	auto res = invper * a;
	res /= res.w;
	auto rev_a = perspective * res;
	return 0;
}