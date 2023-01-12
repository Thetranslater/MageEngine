#include<core/math/matrix4x4.h>

namespace Mage {

	const Matrix4x4 Matrix4x4::zero(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	const Matrix4x4 Matrix4x4::identity(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

	Matrix4x4 Matrix4x4::inverse() {
		float m00{ matrix[0][0] }, m01{ matrix[0][1] }, m02{ matrix[0][2] }, m03{ matrix[0][3] };
		float m10{ matrix[1][0] }, m11{ matrix[1][1] }, m12{ matrix[1][2] }, m13{ matrix[1][3] };
		float m20{ matrix[2][0] }, m21{ matrix[2][1] }, m22{ matrix[2][2] }, m23{ matrix[2][3] };
		float m30{ matrix[3][0] }, m31{ matrix[3][1] }, m32{ matrix[3][2] }, m33{ matrix[3][3] };

		float d2x2_0 = m20 * m31 - m30 * m21;
		float d2x2_1 = m20 * m32 - m30 * m22;
		float d2x2_2 = m20 * m33 - m30 * m23;
		float d2x2_3 = m21 * m32 - m31 * m22;
		float d2x2_4 = m21 * m33 - m31 * m23;
		float d2x2_5 = m22 * m33 - m32 * m23;

		float cofactor_0 = m11 * d2x2_5 - m12 * d2x2_4 + m13 * d2x2_3;
		float cofactor_1 = -(m10 * d2x2_5 - m12 * d2x2_2 + m13 * d2x2_1);
		float cofactor_2 = m10 * d2x2_4 - m11 * d2x2_2 + m13 * d2x2_0;
		float cofactor_3 = -(m10 * d2x2_3 - m11 * d2x2_1 + m12 * d2x2_0);

		float reciprocal_determinant = 1.f / (m00 * cofactor_0 + m01 * cofactor_1 + m02 * cofactor_2 + m03 * cofactor_3);

		float im00 = cofactor_0 * reciprocal_determinant;
		float im10 = cofactor_1 * reciprocal_determinant;
		float im20 = cofactor_2 * reciprocal_determinant;
		float im30 = cofactor_3 * reciprocal_determinant;

		//first col done

		float im01 = -(m01 * d2x2_5 - m02 * d2x2_4 + m03 * d2x2_3) * reciprocal_determinant;
		float im11 = (m00 * d2x2_5 - m02 * d2x2_2 + m03 * d2x2_1) * reciprocal_determinant;
		float im21 = -(m00 * d2x2_4 - m01 * d2x2_2 + m03 * d2x2_0) * reciprocal_determinant;
		float im31 = (m00 * d2x2_3 - m01 * d2x2_1 + m02 * d2x2_0) * reciprocal_determinant;

		//second col done

		d2x2_0 = m00 * m11 - m10 * m01;
		d2x2_1 = m00 * m12 - m10 * m02;
		d2x2_2 = m00 * m13 - m10 * m03;
		d2x2_3 = m01 * m12 - m11 * m02;
		d2x2_4 = m01 * m13 - m11 * m03;
		d2x2_5 = m02 * m13 - m12 * m03;

		float im02 = (m31 * d2x2_5 - m32 * d2x2_4 + m33 * d2x2_3) * reciprocal_determinant;
		float im12 = -(m30 * d2x2_5 - m32 * d2x2_2 + m33 * d2x2_1) * reciprocal_determinant;
		float im22 = (m30 * d2x2_4 - m31 * d2x2_2 + m33 * d2x2_0) * reciprocal_determinant;
		float im32 = -(m30 * d2x2_3 - m31 * d2x2_1 + m32 * d2x2_0) * reciprocal_determinant;

		//third col done

		float im03 = -(m21 * d2x2_5 - m22 * d2x2_4 + m23 * d2x2_3) * reciprocal_determinant;
		float im13 = (m20 * d2x2_5 - m22 * d2x2_2 + m23 * d2x2_1) * reciprocal_determinant;
		float im23 = -(m20 * d2x2_4 - m21 * d2x2_2 + m23 * d2x2_0) * reciprocal_determinant;
		float im33 = (m20 * d2x2_3 - m21 * d2x2_1 + m22 * d2x2_0) * reciprocal_determinant;

		//fourth col done

		return Matrix4x4(im00, im01, im02, im03, im10, im11, im12, im13, im20, im21, im22, im23, im30, im31, im32, im33);
	}

	bool Matrix4x4::Inverse3DAffine(const Matrix4x4& input, Matrix4x4& result) {
		if (!input.ValidTRS()) {
			result = input;
			return false;
		}

		float m00{ input.matrix[0][0] }, m01{ input.matrix[0][1] }, m02{ input.matrix[0][2] }, m03{ input.matrix[0][3] };
		float m10{ input.matrix[1][0] }, m11{ input.matrix[1][1] }, m12{ input.matrix[1][2] }, m13{ input.matrix[1][3] };
		float m20{ input.matrix[2][0] }, m21{ input.matrix[2][1] }, m22{ input.matrix[2][2] }, m23{ input.matrix[2][3] };

		float d2x2_0 = m10 * m21 - m20 * m11;
		float d2x2_1 = m20 * m12 - m10 * m22;
		float d2x2_2 = m11 * m22 - m21 * m12;

		float reciprocal_determinant = 1.f / (m00 * d2x2_2 + m01 * d2x2_1 + m02 * d2x2_0);

		d2x2_0 *= reciprocal_determinant;
		d2x2_1 *= reciprocal_determinant;
		d2x2_2 *= reciprocal_determinant;

		result.matrix[0][0] = d2x2_2;
		result.matrix[1][0] = d2x2_1;
		result.matrix[2][0] = d2x2_0;

		result.matrix[0][1] = (m21 * m02 - m01 * m22) * reciprocal_determinant;
		result.matrix[1][1] = (m00 * m22 - m20 * m02) * reciprocal_determinant;
		result.matrix[2][1] = (m20 * m01 - m00 * m21) * reciprocal_determinant;

		result.matrix[0][2] = (m01 * m12 - m11 * m02) * reciprocal_determinant;
		result.matrix[1][2] = (m10 * m02 - m00 * m12) * reciprocal_determinant;
		result.matrix[2][2] = (m00 * m11 - m10 * m01) * reciprocal_determinant;

		result.matrix[0][3] = -(m03 * d2x2_2 + m13 * result.matrix[0][1] + m23 * result.matrix[0][2]);
		result.matrix[1][3] = -(m03 * d2x2_1 + m13 * result.matrix[1][1] + m23 * result.matrix[1][2]);
		result.matrix[2][3] = -(m03 * d2x2_0 + m13 * result.matrix[2][1] + m23 * result.matrix[2][2]);

		result.matrix[3][0] = 0.f;
		result.matrix[3][1] = 0.f;
		result.matrix[3][2] = 0.f;
		result.matrix[3][3] = 1.f;

		return true;
	}

	Matrix4x4 Matrix4x4::Perspective(float fov, float aspect, float zNear, float zFar) {
		//fov is in degree
		float tan_half_fov = Mathf::Tan(fov / 2.f * Mathf::Deg2Rad);
		Matrix4x4 matrix_perspective = Matrix4x4::zero;
		matrix_perspective[0][0] = 1.f / (tan_half_fov * aspect);
		matrix_perspective[1][1] = 1.f / tan_half_fov;
		matrix_perspective[2][2] = -zFar / (zNear - zFar);
		matrix_perspective[2][3] = (zNear * zFar) / (zNear - zFar);
		matrix_perspective[3][2] = 1.f;
		return matrix_perspective;
	}

	Matrix4x4 Matrix4x4::Ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
		float reciprocal_width  = 1.f / (right - left);
		float reciprocal_height = 1.f / (top - bottom);
		float reciprocal_depth  = 1.f / (zFar - zNear);

		Matrix4x4 matrix_ortho = Matrix4x4::zero;
		matrix_ortho[0][0] =  2.f * reciprocal_width;
		matrix_ortho[1][1] =  2.f * reciprocal_height;
		matrix_ortho[2][2] = -2.f * reciprocal_depth;
		matrix_ortho[0][3] = -(right + left) * reciprocal_width;
		matrix_ortho[1][3] = -(top + bottom) * reciprocal_height;
		matrix_ortho[2][3] = -(zFar + zNear) * reciprocal_height;
		matrix_ortho[3][3] = 1;
		return matrix_ortho;
	}
}