#include<core/math/quaternion.h>
#include<core/math/vector3.h>

namespace Mage {

	const Quaternion Quaternion::zero(0.f, 0.f, 0.f, 0.f);
	const Quaternion Quaternion::identity(0.f, 0.f, 0.f, 1.f); //means no rotation

	//use two directions to set up a rotation quaternion
	void Quaternion::SetFromToRotation(const Vector3& fromDirection, const Vector3& toDirection) {
		Vector3 axis = Vector3::Cross(fromDirection, toDirection);
		axis.Normalize();

		float half_angle = 0.5f * Vector3::Angle(fromDirection, toDirection);
		float sin_half_angle = Mathf::Sin(Mathf::Deg2Rad * half_angle);
		x = axis.x * sin_half_angle;
		y = axis.y * sin_half_angle;
		z = axis.z * sin_half_angle;
		w = Mathf::Cos(half_angle * Mathf::Deg2Rad);
	}

	//TODO: Use cross calculate, not pilot version
	void Quaternion::SetLookRotation(const Vector3& view, const Vector3& up) {
		Vector3 forward_direction = view;
		forward_direction.z = 0.f;
		forward_direction.Normalize();

		Vector3 left_direction = Vector3::Cross(up, forward_direction);

		float matrix[3][3];
		matrix[0][0] = left_direction.x;
		matrix[1][0] = left_direction.y;
		matrix[2][0] = left_direction.z;

		matrix[0][1] = (-forward_direction).x;
		matrix[1][1] = (-forward_direction).y;
		matrix[2][1] = (-forward_direction).z;

		matrix[0][2] = up.x;
		matrix[1][2] = up.y;
		matrix[2][2] = up.z;

		float trace = matrix[0][0] + matrix[1][1] + matrix[2][2];
		float root;
		if (trace > 0.f) {
			root = std::sqrt(trace + 1.f);
			w = 0.5f * root;
			root = 0.5f / root;
			x = (matrix[2][1] - matrix[1][2]) * root;
			y = (matrix[0][2] - matrix[2][0]) * root;
			z = (matrix[1][0] - matrix[0][1]) * root;
		}
		else {
			size_t s_iNext[3] = { 1,2,0 };
			size_t i = 0;
			if (matrix[1][1] > matrix[0][0])
				i = 1;
			if (matrix[2][2] > matrix[i][i])
				i = 2;
			size_t j = s_iNext[i];
			size_t k = s_iNext[j];
			root = std::sqrt(matrix[i][i] - matrix[j][j] - matrix[k][k] + 1.f);
			float* apkQuat[3] = { &x,&y,&z };
			*apkQuat[i] = 0.5f * root;
			root = 0.5f / root;
			w = (matrix[k][j] - matrix[j][k]) * root;
			*apkQuat[j] = (matrix[j][i] + matrix[i][j]) * root;
			*apkQuat[k] = (matrix[k][i] + matrix[i][k]) * root;
		}
		Normalize();
	}

	void Quaternion::ToAngleAxis(float& angle, Vector3& axis) {
		float sqrt = x * x + y * y + z * z;
		if (sqrt > 0.f) {
			angle = 2.f * Mathf::Acos(w) * Mathf::Rad2Deg;
			float reciprocal_sqrt = 1.f / Mathf::Sqrt(sqrt);
			axis.x = x * reciprocal_sqrt;
			axis.y = y * reciprocal_sqrt;
			axis.z = z * reciprocal_sqrt;
		}
		else {
			//angle equals zero, so return in any axis and use (0,0,1) as default;
			angle = 0.f;
			axis.x = 0.f;
			axis.y = 0.f;
			axis.z = 1.f;
		}
	}

	Quaternion Quaternion::FromToRotation(const Vector3& fromDirection, const Vector3& toDirection) {
		Vector3 axis = Vector3::Cross(fromDirection, toDirection);
		axis.Normalize();

		float half_angle = 0.5f * Vector3::Angle(fromDirection, toDirection);
		float sin_half_angle = Mathf::Sin(Mathf::Deg2Rad * half_angle);

		Quaternion result;
		result.w = Mathf::Cos(half_angle * Mathf::Deg2Rad);
		result.x = axis.x * sin_half_angle;
		result.y = axis.y * sin_half_angle;
		result.z = axis.z * sin_half_angle;
		return result;
	}

	//return in degress
	Vector3 Quaternion::ToEulerAngles() {
		Vector3 angles;
		//help variant
		float dy = 2 * y;
		float dz = 2 * z;
		float dx = 2 * x;
		float dw = 2 * w;
		float ww = w * w;
		float xx = x * x;
		float yy = y * y;
		float zz = z * z;

		//yaw
		angles.z = Mathf::Atan2(dy * x + dz * w, ww + xx - yy - zz) * Mathf::Rad2Deg;
		//pitch
		angles.y = Mathf::Asin(-x * dz + w * dy) * Mathf::Rad2Deg;
		//roll
		angles.x = Mathf::Atan2(dw * x + dy * z, ww - xx - yy + zz) * Mathf::Rad2Deg;

		return angles;
	}

	//use in degrees
	//误差！假如我quaternion转成euler，再用该euler转会quaternion，赋值时会在10^{-8}处发生浮动
	void Quaternion::SetEulerAngels(const Vector3& euler) {
		//helper
		float cy = Mathf::Cos(euler.z * 0.5f * Mathf::Deg2Rad);
		float sy = Mathf::Sin(euler.z * 0.5f * Mathf::Deg2Rad);
		float cp = Mathf::Cos(euler.y * 0.5f * Mathf::Deg2Rad);
		float sp = Mathf::Sin(euler.y * 0.5f * Mathf::Deg2Rad);
		float cr = Mathf::Cos(euler.x * 0.5f * Mathf::Deg2Rad);
		float sr = Mathf::Sin(euler.x * 0.5f * Mathf::Deg2Rad);

		w = cr * cp * cy + sr * sp * sy;
		x = sr * cp * cy - cr * sp * sy;
		y = cr * sp * cy + sr * cp * sy;
		z = cr * cp * sy - sr * sp * cy;
	}

	Vector3 Quaternion::operator*(const Vector3& direction) const {
		Vector3 uxv, uxuxv;
		Vector3 vec(x, y, z);
		uxv = Vector3::Cross(vec, direction);
		uxuxv = Vector3::Cross(vec, uxv);
		uxv *= (2.f * w);
		uxuxv *= 2.f;
		return  direction + uxv + uxuxv;
	}

	Quaternion Quaternion::AngleAxis(float angle, const Vector3& axis) {
		Quaternion ret;
		ret.w = Mathf::Cos(angle * 0.5f * Mathf::Deg2Rad);

		float sin_half_angle = Mathf::Sin(angle * 0.5f * Mathf::Deg2Rad);
		ret.x = sin_half_angle * axis.x;
		ret.y = sin_half_angle * axis.y;
		ret.z = sin_half_angle * axis.z;
		return ret;
	}
}