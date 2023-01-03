#pragma once

#include<glm-master/glm/mat4x4.hpp>

#include<core/math/math.h>
#include<core/math/vector3.h>
#include<core/math/vector4.h>
#include<core/math/quaternion.h>
//#include"core/meta/reflection/reflection.h"

#include<cassert>

namespace Mage {
	class Matrix4x4 {
	public:
		float matrix[4][4];
	public:
		//constructor
		Matrix4x4() = default;
		Matrix4x4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) {
			matrix[0][0] =m00, matrix[0][1] =m01, matrix[0][2] =m02, matrix[0][3] =m03 ;
			matrix[1][0] =m10, matrix[1][1] =m11, matrix[1][2] =m12, matrix[1][3] =m13 ;
			matrix[2][0] =m20, matrix[2][1] =m21, matrix[2][2] =m22, matrix[2][3] =m23 ;
			matrix[3][0] =m30, matrix[3][1] =m31, matrix[3][2] =m32, matrix[3][3] =m33 ;
		}
		Matrix4x4(const Matrix4x4& r) {
			matrix[0][0] = r[0][0], matrix[0][1] = r[0][1], matrix[0][2] = r[0][2], matrix[0][3] = r[0][3];
			matrix[1][0] = r[1][0], matrix[1][1] = r[1][1], matrix[1][2] = r[1][2], matrix[1][3] = r[1][3];
			matrix[2][0] = r[2][0], matrix[2][1] = r[2][1], matrix[2][2] = r[2][2], matrix[2][3] = r[2][3];
			matrix[3][0] = r[3][0], matrix[3][1] = r[3][1], matrix[3][2] = r[3][2], matrix[3][3] = r[3][3];
		}
		Matrix4x4(const std::vector<float>& data) :
			Matrix4x4(data[0], data[4], data[8], data[12],
				data[1], data[5], data[9] , data[13],
				data[2], data[6], data[10], data[14],
				data[3], data[7], data[11], data[15]) {}

		//destructor
		~Matrix4x4() = default;

#pragma region PublicMemberFunc

		float determinant() const{
			return matrix[0][0] * cofactor(1, 2, 3, 1, 2, 3) - 
				matrix[0][1] * cofactor(1, 2, 3, 0, 2, 3) + 
				matrix[0][2] * cofactor(1, 2, 3, 0, 1, 3) - 
				matrix[0][3] * cofactor(1, 2, 3, 0, 1, 2);
		}

		//A^{-1} = \frac{A^{*}}{|A|} ::LaTeX representing
		Matrix4x4 inverse();

		bool isIdentity() const{
			return (matrix[0][0] == 1.f && matrix[1][1] == 1.f && matrix[2][2] == 1.f && matrix[3][3] == 1.f) &&
				(matrix[0][1] == 0.f && matrix[0][2] == 0.f && matrix[0][3] == 0.f) &&
				(matrix[1][0] == 0.f && matrix[1][2] == 0.f && matrix[1][3] == 0.f) &&
				(matrix[2][0] == 0.f && matrix[2][1] == 0.f && matrix[2][3] == 0.f) &&
				(matrix[3][0] == 0.f && matrix[3][1] == 0.f && matrix[3][2] == 0.f);
		}

		Matrix4x4 transpose() const{
			return Matrix4x4(
				matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
				matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
				matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
				matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]);
		}

		Vector3 GetPosition() const{
			return Vector3(matrix[0][3], matrix[1][3], matrix[2][3]);
		}

		Vector4 GetColumn(size_t index) const{
			return Vector4(matrix[0][index], matrix[1][index], matrix[2][index], matrix[3][index]);
		}

		Vector4 GetRow(size_t index) const{
			return Vector4(matrix[index][0], matrix[index][1], matrix[index][2], matrix[index][3]);
		}

		void SetColumn(size_t index, const Vector4& column) {
			matrix[0][index] = column.x;
			matrix[1][index] = column.y;
			matrix[2][index] = column.z;
			matrix[3][index] = column.w;
		}

		void SetRow(size_t index, const Vector4& column) {
			matrix[index][0] = column.x;
			matrix[index][1] = column.y;
			matrix[index][2] = column.z;
			matrix[index][3] = column.w;
		}
		//column-major
		void SetMatrix(const std::vector<float>& data) {
			matrix[0][0] = data[0], matrix[0][1] = data[4], matrix[0][2] = data[8], matrix[0][3] = data[12];
			matrix[1][0] = data[1], matrix[1][1] = data[5], matrix[1][2] = data[9], matrix[1][3] = data[13];
			matrix[2][0] = data[2], matrix[2][1] = data[6], matrix[2][2] = data[10], matrix[2][3] = data[14];
			matrix[3][0] = data[3], matrix[3][1] = data[7], matrix[3][2] = data[11], matrix[3][3] = data[15];
		}

		void SetMatrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) {
			matrix[0][0] = m00, matrix[0][1] = m01, matrix[0][2] = m02, matrix[0][3] = m03;
			matrix[1][0] = m10, matrix[1][1] = m11, matrix[1][2] = m12, matrix[1][3] = m13;
			matrix[2][0] = m20, matrix[2][1] = m21, matrix[2][2] = m22, matrix[2][3] = m23;
			matrix[3][0] = m30, matrix[3][1] = m31, matrix[3][2] = m32, matrix[3][3] = m33;
		}

		void SetMatrix(const Matrix4x4& r) {
			matrix[0][0] = r[0][0], matrix[0][1] = r[0][1], matrix[0][2] = r[0][2], matrix[0][3] = r[0][3];
			matrix[1][0] = r[1][0], matrix[1][1] = r[1][1], matrix[1][2] = r[1][2], matrix[1][3] = r[1][3];
			matrix[2][0] = r[2][0], matrix[2][1] = r[2][1], matrix[2][2] = r[2][2], matrix[2][3] = r[2][3];
			matrix[3][0] = r[3][0], matrix[3][1] = r[3][1], matrix[3][2] = r[3][2], matrix[3][3] = r[3][3];
		}

		//make a transformation matrix through "postion rotation and the scale"
		void SetTRS(const Vector3& pos, const Quaternion& q, const Vector3& s) {
			float dx = q.x + q.x;//2x
			float dy = q.y + q.y;//2y
			float dz = q.z + q.z;//2z
			float dxw = dx * q.w;//2xw
			float dyw = dy * q.w;//2yw
			float dzw = dz * q.w;//2zw
			float dxx = dx * q.x;//2x^2
			float dyy = dy * q.y;//2y^2
			float dzz = dz * q.z;//2z^2
			float dxy = dx * q.y;//2xy
			float dxz = dx * q.z;//2xz
			float dyz = dy * q.z;//2yz

			matrix[0][0] = s.x * (1 - (dyy + dzz));
			matrix[0][1] = s.y * (dxy - dzw);
			matrix[0][2] = s.z * (dxz + dyw);
			matrix[0][3] = pos.x;
			matrix[1][0] = s.x * (dxy + dzw);
			matrix[1][1] = s.y * (1 - (dxx + dzz));
			matrix[1][2] = s.z * (dyz - dxw);
			matrix[1][3] = pos.y;
			matrix[2][0] = s.x * (dxz - dyw);
			matrix[2][1] = s.y * (dyz + dxw);
			matrix[2][2] = s.z * (1 - (dxx + dyy));
			matrix[2][3] = pos.z;

			matrix[3][0] = 0.f;
			matrix[3][1] = 0.f;
			matrix[3][2] = 0.f;
			matrix[3][3] = 1.f;
		}

		float* operator[](size_t index) {
			return matrix[index];
		}

		const float* operator[](size_t index) const{
			return matrix[index];
		}

		//only use the rotation part,the vector represents a direction
		Vector3 MultiplyVector(const Vector3& vector) const{
			return Vector3(
				vector.x * matrix[0][0] + vector.y * matrix[0][1] + vector.z * matrix[0][2],
				vector.x * matrix[1][0] + vector.y * matrix[1][1] + vector.z * matrix[1][2],
				vector.x * matrix[2][0] + vector.y * matrix[2][1] + vector.z * matrix[2][2]
			);
		}

		//use all parts of matrix except the perspective
		Vector3 MultiplyPoint3x4(const Vector3& point) const{
			return Vector3(
				point.x * matrix[0][0] + point.y * matrix[0][1] + point.z * matrix[0][2] + matrix[0][3],
				point.x * matrix[1][0] + point.y * matrix[1][1] + point.z * matrix[1][2] + matrix[1][3],
				point.x * matrix[2][0] + point.y * matrix[2][1] + point.z * matrix[2][2] + matrix[2][3]);
		}

		bool ValidTRS() const{
			return matrix[3][0] == 0.f && matrix[3][1] == 0.f && matrix[3][2] == 0.f && matrix[3][3] == 1.f;
		}

#pragma endregion

#pragma region StaticFunc

		static bool Inverse3DAffine(const Matrix4x4& input, Matrix4x4& result);

		//基于摄像机坐标，zNear和zFar会给出偏移量，而非平面具体坐标，因为这样可以提高可读性。摄像机坐标空间采用OPENGL形式，可以于vulkan渲染系统对接。
		//如果同世界坐标系，就需要进行额外的转换，比较麻烦。
		static Matrix4x4 Perspective(float fov/*in degrees*/, float aspect, float zNear, float zFar);

		static Matrix4x4 Ortho(float left, float right, float bottom, float top, float zNear, float zFar);

		static Matrix4x4 Translate(const Vector3& position) {
			return Matrix4x4(
				1, 0, 0, position.x,
				0, 1, 0, position.y,
				0, 0, 1, position.z,
				0, 0, 0,		  1);
		}

		static Matrix4x4 Scale(const Vector3& scale) {
			return Matrix4x4(
				scale.x, 0, 0, 0,
				0, scale.y, 0, 0,
				0, 0, scale.z, 0,
				0, 0,       0, 1);
		}

		static Matrix4x4 Rotate(const Quaternion& q) {
			float dx = q.x + q.x;//2x
			float dy = q.y + q.y;//2y
			float dz = q.z + q.z;//2z
			float dxw = dx * q.w;//2xw
			float dyw = dy * q.w;//2yw
			float dzw = dz * q.w;//2zw
			float dxx = dx * q.x;//2x^2
			float dyy = dy * q.y;//2y^2
			float dzz = dz * q.z;//2z^2
			float dxy = dx * q.y;//2xy
			float dxz = dx * q.z;//2xz
			float dyz = dy * q.z;//2yz

			return Matrix4x4(
				1 - (dyy + dzz), dxy - dzw, dxz + dyw, 0,
				dxy + dzw, 1 - (dxx + dzz), dyz - dxw, 0,
				dxz - dyw, dyz + dxw, 1 - (dxx + dyy), 0,
				        0,         0,               0, 1
			);
		}

		static Matrix4x4 TRS(const Vector3& pos, const Quaternion& q, const Vector3& s) {
			float dx = q.x + q.x;//2x
			float dy = q.y + q.y;//2y
			float dz = q.z + q.z;//2z
			float dxw = dx * q.w;//2xw
			float dyw = dy * q.w;//2yw
			float dzw = dz * q.w;//2zw
			float dxx = dx * q.x;//2x^2
			float dyy = dy * q.y;//2y^2
			float dzz = dz * q.z;//2z^2
			float dxy = dx * q.y;//2xy
			float dxz = dx * q.z;//2xz
			float dyz = dy * q.z;//2yz


			Matrix4x4 matrix_TRS;

			matrix_TRS.matrix[0][0] = s.x * (1 - (dyy + dzz));
			matrix_TRS.matrix[0][1] = s.y * (dxy - dzw);
			matrix_TRS.matrix[0][2] = s.z * (dxz + dyw);
			matrix_TRS.matrix[0][3] = pos.x;
			matrix_TRS.matrix[1][0] = s.x * (dxy + dzw);
			matrix_TRS.matrix[1][1] = s.y * (1 - (dxx + dzz));
			matrix_TRS.matrix[1][2] = s.z * (dyz - dxw);
			matrix_TRS.matrix[1][3] = pos.y;
			matrix_TRS.matrix[2][0] = s.x * (dxz - dyw);
			matrix_TRS.matrix[2][1] = s.y * (dyz + dxw);
			matrix_TRS.matrix[2][2] = s.z * (1 - (dxx + dyy));
			matrix_TRS.matrix[2][3] = pos.z;

			matrix_TRS.matrix[3][0] = 0.f;
			matrix_TRS.matrix[3][1] = 0.f;
			matrix_TRS.matrix[3][2] = 0.f;
			matrix_TRS.matrix[3][3] = 1.f;

			return matrix_TRS;
		}
#pragma endregion

		//operators
		friend Matrix4x4 operator+(const Matrix4x4& lhs, const Matrix4x4& rhs) {
			return Matrix4x4(
				lhs.matrix[0][0] + rhs.matrix[0][0],
				lhs.matrix[0][1] + rhs.matrix[0][1],
				lhs.matrix[0][2] + rhs.matrix[0][2],
				lhs.matrix[0][3] + rhs.matrix[0][3],
				lhs.matrix[1][0] + rhs.matrix[1][0],
				lhs.matrix[1][1] + rhs.matrix[1][1],
				lhs.matrix[1][2] + rhs.matrix[1][2],
				lhs.matrix[1][3] + rhs.matrix[1][3],
				lhs.matrix[2][0] + rhs.matrix[2][0],
				lhs.matrix[2][1] + rhs.matrix[2][1],
				lhs.matrix[2][2] + rhs.matrix[2][2],
				lhs.matrix[2][3] + rhs.matrix[2][3],
				lhs.matrix[3][0] + rhs.matrix[3][0],
				lhs.matrix[3][1] + rhs.matrix[3][1],
				lhs.matrix[3][2] + rhs.matrix[3][2],
				lhs.matrix[3][3] + rhs.matrix[3][3]);
		}
		friend Matrix4x4 operator-(const Matrix4x4& lhs, const Matrix4x4& rhs) {
			return Matrix4x4(
				lhs.matrix[0][0] - rhs.matrix[0][0],
				lhs.matrix[0][1] - rhs.matrix[0][1],
				lhs.matrix[0][2] - rhs.matrix[0][2],
				lhs.matrix[0][3] - rhs.matrix[0][3],
				lhs.matrix[1][0] - rhs.matrix[1][0],
				lhs.matrix[1][1] - rhs.matrix[1][1],
				lhs.matrix[1][2] - rhs.matrix[1][2],
				lhs.matrix[1][3] - rhs.matrix[1][3],
				lhs.matrix[2][0] - rhs.matrix[2][0],
				lhs.matrix[2][1] - rhs.matrix[2][1],
				lhs.matrix[2][2] - rhs.matrix[2][2],
				lhs.matrix[2][3] - rhs.matrix[2][3],
				lhs.matrix[3][0] - rhs.matrix[3][0],
				lhs.matrix[3][1] - rhs.matrix[3][1],
				lhs.matrix[3][2] - rhs.matrix[3][2],
				lhs.matrix[3][3] - rhs.matrix[3][3]);
		}
		friend Matrix4x4 operator*(const Matrix4x4& lhs, const float scale) {
			return Matrix4x4(
				scale * lhs.matrix[0][0],
				scale * lhs.matrix[0][1],
				scale * lhs.matrix[0][2],
				scale * lhs.matrix[0][3],
				scale * lhs.matrix[1][0],
				scale * lhs.matrix[1][1],
				scale * lhs.matrix[1][2],
				scale * lhs.matrix[1][3],
				scale * lhs.matrix[2][0],
				scale * lhs.matrix[2][1],
				scale * lhs.matrix[2][2],
				scale * lhs.matrix[2][3],
				scale * lhs.matrix[3][0],
				scale * lhs.matrix[3][1],
				scale * lhs.matrix[3][2],
				scale * lhs.matrix[3][3]);
		}
		friend Matrix4x4 operator*(const float scale, const Matrix4x4& rhs) {
			return Matrix4x4(
				scale * rhs.matrix[0][0],
				scale * rhs.matrix[0][1],
				scale * rhs.matrix[0][2],
				scale * rhs.matrix[0][3],
				scale * rhs.matrix[1][0],
				scale * rhs.matrix[1][1],
				scale * rhs.matrix[1][2],
				scale * rhs.matrix[1][3],
				scale * rhs.matrix[2][0],
				scale * rhs.matrix[2][1],
				scale * rhs.matrix[2][2],
				scale * rhs.matrix[2][3],
				scale * rhs.matrix[3][0],
				scale * rhs.matrix[3][1],
				scale * rhs.matrix[3][2],
				scale * rhs.matrix[3][3]);
		}
		Matrix4x4 operator*(const Matrix4x4& rhs) const{
			float m00{ rhs.matrix[0][0] }, m01{ rhs.matrix[0][1] }, m02{ rhs.matrix[0][2] }, m03{ rhs.matrix[0][3] };
			float m10{ rhs.matrix[1][0] }, m11{ rhs.matrix[1][1] }, m12{ rhs.matrix[1][2] }, m13{ rhs.matrix[1][3] };
			float m20{ rhs.matrix[2][0] }, m21{ rhs.matrix[2][1] }, m22{ rhs.matrix[2][2] }, m23{ rhs.matrix[2][3] };
			float m30{ rhs.matrix[3][0] }, m31{ rhs.matrix[3][1] }, m32{ rhs.matrix[3][2] }, m33{ rhs.matrix[3][3] };

			return Matrix4x4(
				m00 * matrix[0][0] + m10 * matrix[0][1] + m20 * matrix[0][2] + m30 * matrix[0][3],
				m01 * matrix[0][0] + m11 * matrix[0][1] + m21 * matrix[0][2] + m31 * matrix[0][3],
				m02 * matrix[0][0] + m12 * matrix[0][1] + m22 * matrix[0][2] + m32 * matrix[0][3],
				m03 * matrix[0][0] + m13 * matrix[0][1] + m23 * matrix[0][2] + m33 * matrix[0][3],
				m00 * matrix[1][0] + m10 * matrix[1][1] + m20 * matrix[1][2] + m30 * matrix[1][3],
				m01 * matrix[1][0] + m11 * matrix[1][1] + m21 * matrix[1][2] + m31 * matrix[1][3],
				m02 * matrix[1][0] + m12 * matrix[1][1] + m22 * matrix[1][2] + m32 * matrix[1][3],
				m03 * matrix[1][0] + m13 * matrix[1][1] + m23 * matrix[1][2] + m33 * matrix[1][3],
				m00 * matrix[2][0] + m10 * matrix[2][1] + m20 * matrix[2][2] + m30 * matrix[2][3],
				m01 * matrix[2][0] + m11 * matrix[2][1] + m21 * matrix[2][2] + m31 * matrix[2][3],
				m02 * matrix[2][0] + m12 * matrix[2][1] + m22 * matrix[2][2] + m32 * matrix[2][3],
				m03 * matrix[2][0] + m13 * matrix[2][1] + m23 * matrix[2][2] + m33 * matrix[2][3],
				m00 * matrix[3][0] + m10 * matrix[3][1] + m20 * matrix[3][2] + m30 * matrix[3][3],
				m01 * matrix[3][0] + m11 * matrix[3][1] + m21 * matrix[3][2] + m31 * matrix[3][3],
				m02 * matrix[3][0] + m12 * matrix[3][1] + m22 * matrix[3][2] + m32 * matrix[3][3],
				m03 * matrix[3][0] + m13 * matrix[3][1] + m23 * matrix[3][2] + m33 * matrix[3][3]
				);
		}
		Matrix4x4& operator*=(const Matrix4x4& rhs) {
			*this = *this * rhs;
			return *this;
		}
		Matrix4x4& operator+=(const Matrix4x4& rhs) {
			*this = *this + rhs;
			return *this;
		}
		Vector4 operator*(const Vector4& rhs) const{
			return Vector4(
				rhs.x * matrix[0][0] + rhs.y * matrix[0][1] + rhs.z * matrix[0][2] + rhs.w * matrix[0][3],
				rhs.x * matrix[1][0] + rhs.y * matrix[1][1] + rhs.z * matrix[1][2] + rhs.w * matrix[1][3],
				rhs.x * matrix[2][0] + rhs.y * matrix[2][1] + rhs.z * matrix[2][2] + rhs.w * matrix[2][3],
				rhs.x * matrix[3][0] + rhs.y * matrix[3][1] + rhs.z * matrix[3][2] + rhs.w * matrix[3][3]);
		}
		//this is a perspective transformation, affected in rotation,scale, postion,and perspective
		Vector3 operator*(const Vector3& rhs) const{
			float reciprocal_w = 1.f / (rhs.x * matrix[3][0] + rhs.y * matrix[3][1] + rhs.z * matrix[3][2] + matrix[3][3]);
			return Vector3(
				(rhs.x * matrix[0][0] + rhs.y * matrix[0][1] + rhs.z * matrix[0][2] + matrix[0][3]) * reciprocal_w,
				(rhs.x * matrix[1][0] + rhs.y * matrix[1][1] + rhs.z * matrix[1][2] + matrix[1][3]) * reciprocal_w,
				(rhs.x * matrix[2][0] + rhs.y * matrix[2][1] + rhs.z * matrix[2][2] + matrix[2][3]) * reciprocal_w);
		}
		
		operator glm::mat4x4() const {
			//our matrix keep data in row-major, but glm::mat in column-major
			return glm::mat4x4(	matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
								matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
								matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
								matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]);
		}
		operator glm::mat4x4() {
			//our matrix keep data in row-major, but glm::mat in column-major
			return glm::mat4x4(matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0],
				matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1],
				matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
				matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]);
		}

		//properties
		static const Matrix4x4 zero;
		static const Matrix4x4 identity;


	private:
		//Utilities functions
		//代数余子式
		float cofactor (size_t row1, size_t row2, size_t row3, size_t col1, size_t col2, size_t col3) const{
			return matrix[row1][col1] * (matrix[row2][col2] * matrix[row3][col3] - matrix[row3][col2] * matrix[row2][col3]) -
				matrix[row1][col2] * (matrix[row2][col1] * matrix[row3][col3] - matrix[row3][col2] * matrix[row2][col3]) +
				matrix[row1][col3] * (matrix[row2][col1] * matrix[row3][col2] - matrix[row3][col1] * matrix[row2][col2]);
		}
	};
}