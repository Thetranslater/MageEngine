#pragma once

#include<vector>

#include<glm-master/glm/gtc/quaternion.hpp>

#include<core/math/math.h>
#include"core/meta/reflection/reflection.h"

namespace Mage {
	class Vector3;
	REFLECTION_TYPE(Quaternion)
	CLASS(Quaternion, Fields) {
		REFLECTION_BODY(Quaternion)
	public:
		float w{ 1.f };
		float x{ 0.f };
		float y{ 0.f };
		float z{ 0.f };
	public:
		//constructor
		Quaternion() = default;
		Quaternion(float _x, float _y, float _z, float _w):w(_w), x(_x), y(_y), z(_z) {}
		Quaternion(const Quaternion& _q) :w(_q.w), x(_q.x), y(_q.y), z(_q.z) {}
		Quaternion(const std::vector<float>& data) :w(data[3]), x(data[0]), y(data[1]), z(data[2]) {}

		//destructor
		~Quaternion() = default;
#pragma region PublicMemberFunc
	public:
		float magnitude() const{
			return std::sqrt(x * x + y * y + z * z + w * w);
		}

		void Set(float new_x, float new_y, float new_z, float new_w) {
			x = new_x;
			y = new_y;
			z = new_z;
			w = new_w;
		}

		//create rotation in world space as default
		void SetFromToRotation(const Vector3& fromDirection, const Vector3& toDirection);

		//TODO:
		void SetLookRotation(const Vector3& view, const Vector3& up);

		void Normalize() {
			float length = std::sqrt(x * x + y * y + z * z + w * w);
			float reciprocal_length = 1.f / length;
			x *= reciprocal_length;
			y *= reciprocal_length;
			z *= reciprocal_length;
			w *= reciprocal_length;
		}

		//converts a rotation to angle-axis representation, angles in degrees
		void ToAngleAxis(float& angle, Vector3& axis);

		/*
		*	欧拉角采用内在旋转表示方法，以local坐标系为基准，旋转顺序为z-y-x.
		*	此旋转方法等价于世界坐标系（父坐标系）旋转顺序为x-y-z的旋转，详情：https://www.cnblogs.com/armme/p/10596697.html
		*	@return: vector3(x,y,z) z-yaw    y-pitch   x-roll
		*	外部使用欧拉角顺序必须为yaw-pitch-roll
		*/

		//convert a quaternion to euler angels represented by a vector3
		//return in degrees
		Vector3 ToEulerAngles();

		//use in degrees
		//assign this quaternion by a euler angles
		void SetEulerAngels(const Vector3& euler);

#pragma endregion

#pragma region StaticFunc

		//do not use this directly, this is an utility to calculate
		static float Dot(const Quaternion& a, const Quaternion& b) {
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		}

		static Quaternion FromToRotation(const Vector3& fromDiretion, const Vector3& toDirection);

		//q*q^{-1} = 1
		static Quaternion Inverse(const Quaternion& rotation) {
			float sqlength = rotation.x * rotation.x + rotation.y * rotation.y + rotation.z * rotation.z + rotation.w * rotation.w;
			if (sqlength > 0.f) {
				float reciprocal_sqlength = 1.f / sqlength;
				return Quaternion(-rotation.x * reciprocal_sqlength, -rotation.y * reciprocal_sqlength, -rotation.z * reciprocal_sqlength, -rotation.w * reciprocal_sqlength);
			}
			return Quaternion::zero;
		}

		//return the degrees between a and b , a and b must be unit quaternion
		static float Angle(const Quaternion& a, const Quaternion& b) {
			float dot = Dot(a, b);
			return Mathf::Approximately(dot, 1.f) ? 0.f : Mathf::Acos(dot) * 2.f * Mathf::Rad2Deg;
		}

		//make sure that axis must be unit length, the function does not do the nomarlize operation
		//use in degrees
		static Quaternion AngleAxis(float angle, const Vector3& axis);

		//un-shortest path
		static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t) {
			t = Mathf::Clamp01(t);
			Quaternion result;
			result = a + t * (a - b);
			result.Normalize();
			return result;
		}

		//unclamped and un-shortest path
		static Quaternion LerpUnclamped(const Quaternion& a, const Quaternion& b, float t) {
			Quaternion result;
			result = a + t * (a - b);
			result.Normalize();
			return result;
		}

		//clamped and shortest path
		static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) {
			t = Mathf::Clamp01(t);
			float cos = Dot(a, b);
			Quaternion shortest;

			if (cos < 0.f) {
				cos = -cos;
				shortest = -b;
			}
			else {
				shortest = b;
			}

			if (Mathf::Approximately(Mathf::Abs(cos), 1.f)) {
				//too close of inverse of each other
				Quaternion result = (1.f - t) * a + t * shortest;
				result.Normalize();
				return result;
			}
			else {
				float sin = Mathf::Sqrt(1 - cos * cos);
				float angle = Mathf::Atan2(sin, cos); //radians;
				float reciprocal_sin = 1.f / sin;
				float first_ratio = Mathf::Sin((1.f - t) * angle) * reciprocal_sin;
				float second_ration = Mathf::Sin(t * angle) * reciprocal_sin;
				return first_ratio * a + second_ration * b;
			}
		}



#pragma endregion
		//operators
		Quaternion operator-(void) const { return Quaternion(-x, -y, -z, -w); }
		Quaternion operator-(const Quaternion& rhs) const { return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
		Quaternion operator+(const Quaternion& rhs) const { return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
		//apply the rotation to a vector, not multi by left
		Vector3 operator*(const Vector3& direction);
		friend Quaternion operator*(float scale, const Quaternion& rhs) {
			return Quaternion(scale * rhs.x, scale * rhs.y, scale * rhs.z, scale * rhs.w);
		}
		Quaternion operator*(const Quaternion& rhs) const {
			return Quaternion(
				y * rhs.z - z * rhs.y + w * rhs.x + rhs.w * x,
				z * rhs.x - x * rhs.z + w * rhs.y + rhs.w * y,
				x * rhs.y - y * rhs.x + w * rhs.z + rhs.w * z,
				w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
		}
		Quaternion operator/(const float scale) const {
			float reciprocal_scale = 1.f / scale;
			return Quaternion(x * reciprocal_scale, y * reciprocal_scale, z * reciprocal_scale, w * reciprocal_scale);
		}
		Quaternion& operator+=(const Quaternion& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}
		Quaternion& operator-=(const Quaternion& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}
		operator glm::quat() { return glm::quat(w, x, y, z); }
		operator glm::quat() const { return glm::quat(w, x, y, z); }
		//friend Vector3 operator*(const Vector3& direction, const Quaternion& rotate);

		//properties
		static const Quaternion zero;
		static const Quaternion identity;

	};
}