#pragma once

#include<cmath>
#include<cassert>
#include<string>
#include<vector>

#include<glm-master/glm/vec2.hpp>

#include<core/math/math.h>

namespace Mage {
	class Vector2 {
	public:
		float x{ 0.f }, y{ 0.f };
	public:
		// constructor
		Vector2() = default;
		Vector2(float _x, float _y) :x(_x), y(_y) {}
		Vector2(const Vector2& _vec) :x(_vec.x), y(_vec.y) {}

		//destructor
		~Vector2() = default;

#pragma region PublicMemberFunc
	public:
		float magnitude() const{
			return std::hypot(x, y);
		}

		float sqrMagnitude() const{
			return x * x + y * y;
		}

		//return true if the 'vec' vector2 is exactly equal to *this
		bool Equals(const Vector2& vec) const{
			return vec.x == x && vec.y == y;
		}

		//return a copy unit vector2 which has the same direction of *this,
		Vector2 normalized() const{
			Vector2 unit(x, y);
			unit.Normalize();
			return unit;
		}

		//make *this into a unit vector2
		void Normalize() {
			float length = std::hypot(x, y);
			if (length > 0.f) {
				float reciprocal_length = 1.f / length;
				x *= reciprocal_length;
				y *= reciprocal_length;
			}
		}

		//write
		void Set(float new_x, float new_y) { x = new_x; y = new_y; }

		//read
		float operator[](size_t index) const{
			assert(index < 2);
			return index == 0 ? x : y;
		}

		//write
		float& operator[](size_t index) {
			assert(index < 2);
			return index == 0 ? x : y;
		}

		//TODO
		std::string ToString(const std::string& format);

		//opeartors 
		Vector2& operator*=(const Vector2& rhs) {
			x *= rhs.x;
			y *= rhs.y;
			return *this;
		}

		Vector2& operator+=(const Vector2& rhs) {
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		Vector2& operator-=(const Vector2& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		Vector2& operator/=(const float scale) {
			float reciprocal_scale = 1.f / scale;
			x *= reciprocal_scale;
			y *= reciprocal_scale;
			return *this;
		}

		Vector2 operator/=(const Vector2& rhs) {
			x /= rhs.x;
			y /= rhs.y;
			return *this;
		}
#pragma endregion

#pragma region StaticFunc
	public:
		//return the unsigned angle between the 'from' and the 'to', its range always in the [0,180.f]
		static float Angle(const Vector2& from, const Vector2& to) {
			return std::acos(Dot(from, to) / (from.magnitude() * to.magnitude())) * Mathf::Rad2Deg;
		}

		//return dot product
		static float Dot(const Vector2& lhs, const Vector2& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }

		//return the distance of two vector2s
		static float Distance(const Vector2& lhs, const Vector2& rhs) { return (lhs - rhs).magnitude(); }

		//lerp without the check
		static Vector2 LerpUnclamped(const Vector2& a, const Vector2& b, float alpha) {
			return a + alpha * (b - a);
		}

		//lerp with the check
		static Vector2 Lerp(const Vector2& a, const Vector2& b, float alpha) {
			alpha = alpha > 1.f ? 1.f : alpha;
			alpha = alpha < 0.f ? 0.f : alpha;
			return a + alpha * (b - a);
		}

		//return the max vector2 which combine with  max components of the two vector2s
		static Vector2 Max(const Vector2& lhs, const Vector2& rhs) {
			return Vector2(lhs.x > rhs.x ? lhs.x : rhs.x, lhs.y > rhs.y ? lhs.y : rhs.y);
		}

		//return the min vector2 which combine with min components of the two vector2s
		static Vector2 Min(const Vector2& lhs, const Vector2& rhs) {
			return Vector2(lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y);
		}

		static Vector2 Scale(const Vector2& lhs, const Vector2& rhs) {
			return Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
		}

		//inDirection is pointing to the plane and the normal must be an unit vector2
		static Vector2 Reflect(const Vector2& inDirection, const Vector2& inNormal) {
			return inDirection - 2 * (Dot(inDirection, inNormal) * inNormal);
		}

		static Vector2 MoveTowards(const Vector2& current, const Vector2& target, float maxDistanceDelta) {
			Vector2 diff(target - current);
			if (maxDistanceDelta <0.f ||(maxDistanceDelta * maxDistanceDelta < diff.sqrMagnitude())) {
				return current + maxDistanceDelta * diff.normalized();
			}
			return target;
		}
#pragma endregion

		//friend operators
		friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs) { return Vector2(lhs.x - rhs.x, lhs.y - rhs.y); }
		friend Vector2 operator+(const Vector2 & lhs, const Vector2 & rhs) { return Vector2(lhs.x + rhs.x, lhs.y + rhs.y); }
		//return true if two vector2s are approximately equal
		friend bool operator==(const Vector2& lhs, const Vector2& rhs); //TODO
		friend Vector2 operator*(float scale, const Vector2& rhs) { return Vector2(scale * rhs.x, scale * rhs.y); }
		friend Vector2 operator*(const Vector2& lhs, const Vector2& rhs) { return Vector2(lhs.x * rhs.x, lhs.y * rhs.y); }
		friend Vector2 operator*(const Vector2& lhs, float scale) { return Vector2(scale * lhs.x, scale * lhs.y); }
		friend Vector2 operator/(const Vector2& lhs, const float scale) {
			float reciprocal_scale = 1.f / scale;
			return Vector2(lhs.x * reciprocal_scale, lhs.y * reciprocal_scale);
		}
		friend Vector2 operator/(const Vector2& lhs, const Vector2& rhs) { return Vector2(lhs.x / rhs.x, lhs.y / rhs.y); }
		operator glm::vec2() { return glm::vec2(x, y); }
		operator glm::vec2() const { return glm::vec2(x, y); }

		//static properties
		static const Vector2 down;
		static const Vector2 left;
		static const Vector2 one;
		static const Vector2 right;
		static const Vector2 up;
		static const Vector2 zero;
	};
}