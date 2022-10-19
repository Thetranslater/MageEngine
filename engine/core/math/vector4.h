#pragma once

#include<cmath>
#include<cassert>
#include<vector>

#include<glm-master/glm/vec4.hpp>

namespace Mage {
	class Vector4 {
	public:
		float x{ 0.f };
		float y{ 0.f };
		float z{ 0.f };
		float w{ 0.f };

	public:
		//constructor
		Vector4() = default;
		Vector4(float _x, float _y, float _z, float _w) :x(_x), y(_y), z(_z), w(_w) {}
		Vector4(const Vector4& _vec) :x(_vec.x), y(_vec.y), z(_vec.z), w(_vec.w) {}
		Vector4(const std::vector<float>& data) :x(data[0]), y(data[1]), z(data[2]), w(data[3]) {}

		//destructor
		~Vector4() = default;

#pragma region PublicMemberFunc

		float magnitude() const { return std::sqrt(x * x + y * y + z * z + w * w); }

		Vector4 normalized() const{
			Vector4 unit(x, y, z, w);
			unit.Normalize();
			return unit;
		}

		void Normalize() {
			float length = std::sqrt(x * x + y * y + z * z + w * w);
			if (length > 0.f) {
				float reciprocal_length = 1.f / length;
				x *= reciprocal_length;
				y *= reciprocal_length;
				z *= reciprocal_length;
				w *= reciprocal_length;
			}
		}

		float sqrtMagnitude() const{
			return x * x + y * y + z * z + w * w;
		}

		bool Equals(const Vector4& vec) const{
			return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
		}

		float operator[](size_t index) const {
			assert(index < 4);
			return *(&x + index);
		}

		float& operator[](size_t index) {
			assert(index < 4);
			return *(&x + index);
		}

		void Set(float new_x, float new_y, float new_z, float new_w) {
			x = new_x;
			y = new_y;
			z = new_z;
			w = new_w;
		}

		Vector4 operator+=(const Vector4& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}

		Vector4 operator-=(const Vector4& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}

		Vector4 operator*=(const Vector4& rhs) {
			x *= rhs.x;
			y *= rhs.y;
			z *= rhs.z;
			w *= rhs.w;
			return *this;
		}

		Vector4 operator/=(const Vector4& rhs) {
			x /= rhs.x;
			y /= rhs.y;
			z /= rhs.z;
			w /= rhs.w;
			return *this;
		}

		Vector4 operator/=(const float scale) {
			float reciprocal_scale = 1.f / scale;
			x *= reciprocal_scale;
			y *= reciprocal_scale;
			z *= reciprocal_scale;
			w *= reciprocal_scale;
			return *this;
		}
#pragma endregion

#pragma region StaticFunc

		float Distance(const Vector4& lhs, const Vector4& rhs) {
			return (rhs - lhs).magnitude();
		}

		float Dot(const Vector4& lhs, const Vector4& rhs) {
			return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
		}

#pragma endregion

		//operators
		friend Vector4 operator+(const Vector4& lhs, const Vector4& rhs) { return Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
		friend Vector4 operator-(const Vector4& lhs, const Vector4& rhs) { return Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
		friend Vector4 operator*(const Vector4& lhs, const float scale) { return Vector4(lhs.x * scale, lhs.y * scale, lhs.z * scale, lhs.w * scale); }
		friend Vector4 operator*(const Vector4& lhs, const Vector4& rhs) { return Vector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.z * rhs.z); }
		friend Vector4 operator*(const float scale, const Vector4& rhs) { return Vector4(scale * rhs.x, scale * rhs.y, scale * rhs.z, scale * rhs.w); }
		friend Vector4 operator/(const Vector4& lhs, const float scale) { 
			float reciprocal_scale = 1.f / scale; 
			return Vector4(lhs.x * reciprocal_scale, lhs.y * reciprocal_scale, lhs.z * reciprocal_scale, lhs.w * reciprocal_scale);
		}
		friend Vector4 operator/(const Vector4& lhs, const Vector4& rhs) { return Vector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w); }
		operator glm::vec4() {
			return glm::vec4(x, y, z, w);
		}
		operator glm::vec4() const{
			return glm::vec4(x, y, z, w);
		}
	};
}