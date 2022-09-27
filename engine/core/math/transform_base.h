#pragma once

#include"math.h"
#include"vector3.h"
#include"matrix4x4.h"
#include"quaternion.h"
#include<cassert>

namespace Mage {

	//use for Transform component, this is core to calculate
	class TransformBase {
	public:
		//these are relative to the parent GO's transform
		Vector3 position{Vector3::zero};
		Vector3 scale{Vector3::one};
		Quaternion rotation{};
	private:
		//cache the transform matrix;
		Matrix4x4 cached_transform_matrix;

	public:
		
	};
}