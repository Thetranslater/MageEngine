#pragma once

#include<cmath>
#include<cassert>

#include<core/math/math.h>

namespace Mage {
	class Vector3 {
	public:
		float x{ 0.f };
		float y{ 0.f };
		float z{ 0.f };
	public:
		//constructor
		Vector3() = default;
		Vector3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {};
		Vector3(const Vector3& _vec) :x(_vec.x), y(_vec.y), z(_vec.z) {};

		//destructor
		~Vector3() = default;

#pragma region PublicMemberFunc
		
		float magnitude() const{ return std::hypot(x, y, z); }

		float sqrMagnitude() const{
			return x * x + y * y + z * z;
		}

		Vector3 normalized() const{
			Vector3 unit(x, y, z);
			unit.Normalize();
			return unit;
		}

		void Normalize() {
			float length = std::hypot(x, y, z);
			if (length > 0.f) {
				float reciprocal_length = 1.f / length;
				x *= reciprocal_length;
				y *= reciprocal_length;
				z *= reciprocal_length;
			}
		}

		void Set(float new_x, float new_y, float new_z) { x = new_x; y = new_y; z = new_z; }

		bool Equals(const Vector3& vec) const{ return x == vec.x && y == vec.y && z == vec.z; }

		float operator[](size_t index) const{
			assert(index < 3);
			return *(&x + index);
		}

		float& operator[](size_t index) {
			assert(index < 3);
			return *(&x + index);
		}

		Vector3 operator-() { return Vector3(-x, -y, -z); }

		Vector3& operator+=(const Vector3& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		Vector3& operator-=(const Vector3& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		Vector3& operator*=(const Vector3& rhs) {
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
			return *this;
		}

		Vector3& operator*=(const float rhs) {
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return *this;
		}

		Vector3 operator/=(const Vector3& rhs) {
			x /= rhs.x;
			y /= rhs.y;
			z /= rhs.z;
			return *this;
		}

		Vector3 operator/=(float scale) {
			float reciprocal_scale = 1.f / scale;
			x *= reciprocal_scale;
			y *= reciprocal_scale;
			z *= reciprocal_scale;
			return *this;
		}
#pragma endregion

#pragma region StaticFunc

		//return in degree
		static float Angle(const Vector3& from, const Vector3& to) {
			return Mathf::Acos((Dot(from, to) / (from.magnitude() * to.magnitude()))) * Mathf::Rad2Deg;
		}

		static Vector3 Cross(const Vector3& lhs, const Vector3& rhs) {
			return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
		}

		static float Dot(const Vector3& lhs, const Vector3& rhs) {
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
		}

		static float Distance(const Vector3& lhs, const Vector3& rhs) {
			return (rhs - lhs).magnitude();
		}

		static Vector3 Max(const Vector3& lhs, const Vector3& rhs) {
			return Vector3(
				lhs.x > rhs.x ? lhs.x : rhs.x,
				lhs.y > rhs.y ? lhs.y : rhs.y,
				lhs.z > rhs.z ? lhs.z : rhs.z
			);
		}

		static Vector3 Min(const Vector3& lhs, const Vector3& rhs) {
			return Vector3(
				lhs.x < rhs.x ? lhs.x : rhs.x,
				lhs.y < rhs.y ? lhs.y : rhs.y,
				lhs.z < rhs.z ? lhs.z : rhs.z
			);
		}

		//lerp without check
		static Vector3 LerpUnclamped(const Vector3& a, const Vector3& b, float alpha) {
			return a + alpha * (b - a);
		}

		//lerp with check
		static Vector3 Lerp(const Vector3& a, const Vector3& b, float alpha) {
			alpha = alpha > 1.f ? 1.f : alpha;
			alpha = alpha < 0.f ? 0.f : alpha;
			return a + alpha * (b - a);
		}

		//TODO:
		static Vector3 Slerp(const Vector3& a, const Vector3& b, float alpha) {
			float length_a = a.magnitude();
			float length_b = b.magnitude();
			Vector3 unit_a = a * (1.f / length_a);
			Vector3 unit_b = b * (1.f / length_b);
			float angle = std::acos(Dot(a, b) / (length_a * length_b));

			alpha = alpha > 1.f ? 1.f : alpha;
			alpha = alpha < 0.f ? 0.f : alpha;

			Vector3 lerp_result_direction = Vector3(std::sin((1 - alpha) * angle) * unit_a + std::sin(alpha * angle) * unit_b) / std::sin(angle);
			lerp_result_direction.Normalize();

			return lerp_result_direction * (length_a + alpha * (length_b - length_a));
		}

		//TODO£º
		static Vector3 SlerpUnclamped(const Vector3& a, const Vector3& b, float alpha);

		static Vector3 MoveTowards(const Vector3& current, const Vector3& target, float maxDistanceDelta) {
			Vector3 diff(target - current);
			if (maxDistanceDelta < 0.f || (maxDistanceDelta * maxDistanceDelta < diff.sqrMagnitude())) {
				return current + maxDistanceDelta * diff.normalized();
			}
			return target;
		}

		static Vector3 Project(const Vector3& vector, const Vector3& normal) {
			return normal * Dot(vector, normal);
		}

		static Vector3 ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal) {
			return vector - (planeNormal * Dot(vector, planeNormal));
		}

		static Vector3 Reflect(const Vector3& inDirection, const Vector3& normal) {
			return inDirection - (2 * Dot(inDirection, normal) * normal);
		}

		static Vector3 Scale(const Vector3& lhs, const Vector3& rhs) {
			return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
		}
#pragma endregion

		//operators
		friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs) { return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
		friend Vector3 operator+(const Vector3& lhs, const Vector3& rhs) { return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
		friend Vector3 operator*(float scale, const Vector3& rhs) { return Vector3(scale * rhs.x, scale * rhs.y, scale * rhs.z); }
		friend Vector3 operator*(const Vector3& lhs, float scale) { return Vector3(scale * lhs.x, scale * lhs.y, scale * lhs.z); };
		friend Vector3 operator*(const Vector3& lhs, const Vector3& rhs) { return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }
		friend Vector3 operator/(const Vector3& lhs, float scale) {
			float reciprocal_scale = 1.f / scale;
			return Vector3(lhs.x * reciprocal_scale, lhs.y * reciprocal_scale, lhs.z * reciprocal_scale);
		}
		friend Vector3 operator/(const Vector3& lhs, const Vector3& rhs) { return Vector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z); }
		//TODO:
		friend Vector3 operator==(const Vector3& lhs, const Vector3& rhs);

		//static properties
		static const Vector3 back;
		static const Vector3 down;
		static const Vector3 forward;
		static const Vector3 left;
		static const Vector3 right;
		static const Vector3 up;
		static const Vector3 zero;
		static const Vector3 one;
	};
}