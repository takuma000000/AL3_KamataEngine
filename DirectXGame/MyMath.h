#pragma once
//#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>
#include <cmath>

class MyMath {

public:

	struct Vector3 {
		float x;
		float y;
		float z;
	};

	struct Matrix4x4 {
		float m[4][4];
	};

	Vector3 Add(const Vector3& v1, const Vector3& v2) {
		Vector3 result;
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;
		return result;
	}

	Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
		Vector3 result;
		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z - v2.z;
		return result;
	}

	Vector3 Multiply(float scalar, const Vector3& v) {
		Vector3 result;
		result.x = scalar * v.x;
		result.y = scalar * v.y;
		result.z = scalar * v.z;
		return result;
	}

	float Dot(const Vector3& v1, const Vector3& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	float Length(const Vector3& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }

	Vector3 Normalize(const Vector3& v) {
		float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

		// ������0�̏ꍇ�A�x�N�g�������̂܂ܕԂ�
		if (length == 0) {
			return v;
		}

		// �e�����𒷂��Ŋ��邱�ƂŐ��K������
		Vector3 normalized;
		normalized.x = v.x / length;
		normalized.y = v.y / length;
		normalized.z = v.z / length;
		return normalized;
	}

	Matrix4x4 Inverse(const Matrix4x4& m) {
		Matrix4x4 result;

		// �s��̗]���q�s����v�Z
		result.m[0][0] = m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
		                 m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][3] * m.m[3][2] -
		                 m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][3] * m.m[2][2] * m.m[3][1];
		result.m[0][1] = m.m[0][1] * m.m[2][3] * m.m[3][2] + m.m[0][2] * m.m[2][1] * m.m[3][3] +
		                 m.m[0][3] * m.m[2][2] * m.m[3][1] - m.m[0][1] * m.m[2][2] * m.m[3][3] -
		                 m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2];
		result.m[0][2] = m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
		                 m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][3] * m.m[3][2] -
		                 m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][3] * m.m[1][2] * m.m[3][1];
		result.m[0][3] = m.m[0][1] * m.m[1][3] * m.m[2][2] + m.m[0][2] * m.m[1][1] * m.m[2][3] +
		                 m.m[0][3] * m.m[1][2] * m.m[2][1] - m.m[0][1] * m.m[1][2] * m.m[2][3] -
		                 m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2];

		result.m[1][0] = m.m[1][0] * m.m[2][3] * m.m[3][2] + m.m[1][2] * m.m[2][0] * m.m[3][3] +
		                 m.m[1][3] * m.m[2][2] * m.m[3][0] - m.m[1][0] * m.m[2][2] * m.m[3][3] -
		                 m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2];
		result.m[1][1] = m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
		                 m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][3] * m.m[3][2] -
		                 m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][3] * m.m[2][2] * m.m[3][0];
		result.m[1][2] = m.m[0][0] * m.m[1][3] * m.m[3][2] + m.m[0][2] * m.m[1][0] * m.m[3][3] +
		                 m.m[0][3] * m.m[1][2] * m.m[3][0] - m.m[0][0] * m.m[1][2] * m.m[3][3] -
		                 m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2];
		result.m[1][3] = m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
		                 m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][3] * m.m[2][2] -
		                 m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][3] * m.m[1][2] * m.m[2][0];

		result.m[2][0] = m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
		                 m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][0] * m.m[2][3] * m.m[3][1] -
		                 m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][3] * m.m[2][1] * m.m[3][0];
		result.m[2][1] = m.m[0][0] * m.m[2][3] * m.m[3][1] + m.m[0][1] * m.m[2][0] * m.m[3][3] +
		                 m.m[0][3] * m.m[2][1] * m.m[3][0] - m.m[0][0] * m.m[2][1] * m.m[3][3] -
		                 m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1];
		result.m[2][2] = m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
		                 m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][0] * m.m[1][3] * m.m[3][1] -
		                 m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][3] * m.m[1][1] * m.m[3][0];
		result.m[2][3] = m.m[0][0] * m.m[1][3] * m.m[2][1] + m.m[0][1] * m.m[1][0] * m.m[2][3] +
		                 m.m[0][3] * m.m[1][1] * m.m[2][0] - m.m[0][0] * m.m[1][1] * m.m[2][3] -
		                 m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1];

		result.m[3][0] = m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[1][1] * m.m[2][0] * m.m[3][2] +
		                 m.m[1][2] * m.m[2][1] * m.m[3][0] - m.m[1][0] * m.m[2][1] * m.m[3][2] -
		                 m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1];
		result.m[3][1] = m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
		                 m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][0] * m.m[2][2] * m.m[3][1] -
		                 m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][2] * m.m[2][1] * m.m[3][0];
		result.m[3][2] = m.m[0][0] * m.m[1][2] * m.m[3][1] + m.m[0][1] * m.m[1][0] * m.m[3][2] +
		                 m.m[0][2] * m.m[1][1] * m.m[3][0] - m.m[0][0] * m.m[1][1] * m.m[3][2] -
		                 m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1];
		result.m[3][3] = m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
		                 m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] -
		                 m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][2] * m.m[1][1] * m.m[2][0];

		// �s�񎮂��v�Z
		float determinant = m.m[0][0] * result.m[0][0] + m.m[0][1] * result.m[1][0] +
		                    m.m[0][2] * result.m[2][0] + m.m[0][3] * result.m[3][0];

		// �s�񎮂�0�̏ꍇ�A�t�s��͑��݂��Ȃ�
		if (determinant == 0) {
			// �G���[�n���h�����O��K�؂ȏ�����ǉ�����
			// �K���ȃf�t�H���g�̍s���Ԃ��ꍇ�Ȃǂ��l�����܂�
			return result; // �[���s���Ԃ����ƂŃG���[������
		}

		// �s��̋t�s����v�Z
		float inverseFactor = 1.0f / determinant;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] *= inverseFactor;
			}
		}

		return result;
	}

	Matrix4x4 Transpose(const Matrix4x4& m) {
		Matrix4x4 result;

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = m.m[j][i];
			}
		}

		return result;
	}

	Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
		Matrix4x4 result;

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = m1.m[i][j] + m2.m[i][j];
			}
		}

		return result;
	}

	Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
		Matrix4x4 result;

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = m1.m[i][j] - m2.m[i][j];
			}
		}

		return result;
	}

	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
		Matrix4x4 result;

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = 0;
				for (int k = 0; k < 4; ++k) {
					result.m[i][j] += m1.m[i][k] * m2.m[k][j];
				}
			}
		}

		return result;
	}


	Matrix4x4 MakeIdentity4x4() {

		Matrix4x4 identity;

		// �P�ʍs��𐶐�����
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				identity.m[i][j] = (i == j) ? 1.0f : 0.0f;
			}
		}

		return identity;
	}

	Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
		Matrix4x4 result;

		// �P�ʍs��ɐݒ肵�܂�
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (i == j) {
					result.m[i][j] = 1.0f;
				} else {
					result.m[i][j] = 0.0f;
				}
			}
		}

		// �ړ�������ݒ肵�܂�
		result.m[3][0] = translate.x;
		result.m[3][1] = translate.y;
		result.m[3][2] = translate.z;

		return result;
	}

	Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
		Matrix4x4 result;

		// �P�ʍs��ɐݒ肵�܂�
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = (i == j) ? 1.0f : 0.0f;
			}
		}

		// �g�听����ݒ肵�܂�
		result.m[0][0] = scale.x;
		result.m[1][1] = scale.y;
		result.m[2][2] = scale.z;

		return result;
	}

	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
		Vector3 result;

		result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] +
		           vector.z * matrix.m[2][0] + matrix.m[3][0];
		result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] +
		           vector.z * matrix.m[2][1] + matrix.m[3][1];
		result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] +
		           vector.z * matrix.m[2][2] + matrix.m[3][2];
		float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] +
		          vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
		assert(w != 0.0f);
		result.x /= w;
		result.y /= w;
		result.z /= w;

		return result;
	}

	Matrix4x4 MakeRotateXMatrix(float radian) {
		float c = std::cos(radian);
		float s = std::sin(radian);

		Matrix4x4 rotationMatrix;

		rotationMatrix.m[0][0] = 1.0f;
		rotationMatrix.m[0][1] = 0.0f;
		rotationMatrix.m[0][2] = 0.0f;
		rotationMatrix.m[0][3] = 0.0f;
		rotationMatrix.m[1][0] = 0.0f;
		rotationMatrix.m[1][1] = c;
		rotationMatrix.m[1][2] = s;
		rotationMatrix.m[1][3] = 0.0f;
		rotationMatrix.m[2][0] = 0.0f;
		rotationMatrix.m[2][1] = -s;
		rotationMatrix.m[2][2] = c;
		rotationMatrix.m[2][3] = 0.0f;
		rotationMatrix.m[3][0] = 0.0f;
		rotationMatrix.m[3][1] = 0.0f;
		rotationMatrix.m[3][2] = 0.0f;
		rotationMatrix.m[3][3] = 1.0f;

		return rotationMatrix;
	}

	Matrix4x4 MakeRotateYMatrix(float radian) {
		float c = std::cos(radian);
		float s = std::sin(radian);

		Matrix4x4 rotationMatrix;

		rotationMatrix.m[0][0] = c;
		rotationMatrix.m[0][1] = 0.0f;
		rotationMatrix.m[0][2] = -s;
		rotationMatrix.m[0][3] = 0.0f;
		rotationMatrix.m[1][0] = 0.0f;
		rotationMatrix.m[1][1] = 1.0f;
		rotationMatrix.m[1][2] = 0.0f;
		rotationMatrix.m[1][3] = 0.0f;
		rotationMatrix.m[2][0] = s;
		rotationMatrix.m[2][1] = 0.0f;
		rotationMatrix.m[2][2] = c;
		rotationMatrix.m[2][3] = 0.0f;
		rotationMatrix.m[3][0] = 0.0f;
		rotationMatrix.m[3][1] = 0.0f;
		rotationMatrix.m[3][2] = 0.0f;
		rotationMatrix.m[3][3] = 1.0f;

		return rotationMatrix;
	}

	Matrix4x4 MakeRotateZMatrix(float radian) {
		float c = std::cos(radian);
		float s = std::sin(radian);

		Matrix4x4 rotationMatrix;

		rotationMatrix.m[0][0] = c;
		rotationMatrix.m[0][1] = s;
		rotationMatrix.m[0][2] = 0.0f;
		rotationMatrix.m[0][3] = 0.0f;
		rotationMatrix.m[1][0] = -s;
		rotationMatrix.m[1][1] = c;
		rotationMatrix.m[1][2] = 0.0f;
		rotationMatrix.m[1][3] = 0.0f;
		rotationMatrix.m[2][0] = 0.0f;
		rotationMatrix.m[2][1] = 0.0f;
		rotationMatrix.m[2][2] = 1.0f;
		rotationMatrix.m[2][3] = 0.0f;
		rotationMatrix.m[3][0] = 0.0f;
		rotationMatrix.m[3][1] = 0.0f;
		rotationMatrix.m[3][2] = 0.0f;
		rotationMatrix.m[3][3] = 1.0f;

		return rotationMatrix;
	}

	Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate) {
		Matrix4x4 affineMatrix;

		// �e�ϊ��s����쐬
		Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

		Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

		// �e�ϊ��s����������ăA�t�B���ϊ��s����쐬
		affineMatrix = Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));

		return affineMatrix;
	}

	Matrix4x4
	    MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
		float tanHalfFovY = tanf(fovY * 0.5f);
		float scaleX = 1.0f / (aspectRatio * tanHalfFovY);
		float scaleY = 1.0f / tanHalfFovY;
		// float nearFarRange = farClip - nearClip;

		Matrix4x4 result;

		result.m[0][0] = scaleX;
		result.m[0][1] = 0.0f;
		result.m[0][2] = 0.0f;
		result.m[0][3] = 0.0f;

		result.m[1][0] = 0.0f;
		result.m[1][1] = scaleY;
		result.m[1][2] = 0.0f;
		result.m[1][3] = 0.0f;

		result.m[2][0] = 0.0f;
		result.m[2][1] = 0.0f;
		result.m[2][2] = farClip / (farClip - nearClip);
		result.m[2][3] = 1.0f;

		result.m[3][0] = 0.0f;
		result.m[3][1] = 0.0f;
		result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
		result.m[3][3] = 0.0f;

		return result;
	}

	Matrix4x4 MakeOrthographicMatrix(
	    float left, float top, float right, float bottom, float nearClip, float farClip) {
		Matrix4x4 mat;

		mat.m[0][0] = 2.0f / (right - left);
		mat.m[0][1] = 0.0f;
		mat.m[0][2] = 0.0f;
		mat.m[0][3] = 0.0f;

		mat.m[1][0] = 0.0f;
		mat.m[1][1] = 2.0f / (top - bottom);
		mat.m[1][2] = 0.0f;
		mat.m[1][3] = 0.0f;

		mat.m[2][0] = 0.0f;
		mat.m[2][1] = 0.0f;
		mat.m[2][2] = 1.0f / (farClip - nearClip);
		mat.m[2][3] = 0.0f;

		mat.m[3][0] = -(right + left) / (right - left);
		mat.m[3][1] = -(top + bottom) / (top - bottom);
		mat.m[3][2] = (nearClip) / (nearClip - farClip);
		mat.m[3][3] = 1.0f;

		return mat;
	}

	Matrix4x4 MakeViewportMatrix(
	    float left, float top, float width, float height, float minDepth, float maxDepth) {
		Matrix4x4 mat;

		mat.m[0][0] = width / 2.0f;
		mat.m[0][1] = 0.0f;
		mat.m[0][2] = 0.0f;
		mat.m[0][3] = 0.0f;

		mat.m[1][0] = 0.0f;
		mat.m[1][1] = -height / 2.0f;
		mat.m[1][2] = 0.0f;
		mat.m[1][3] = 0.0f;

		mat.m[2][0] = 0.0f;
		mat.m[2][1] = 0.0f;
		mat.m[2][2] = maxDepth - minDepth;
		mat.m[2][3] = 0.0f;

		mat.m[3][0] = left + width / 2.0f;
		mat.m[3][1] = top + height / 2.0f;
		mat.m[3][2] = minDepth;
		mat.m[3][3] = 1.0f;

		return mat;
	}

	Vector3 Cross(const Vector3& v1, const Vector3& v2) {
		Vector3 result;
		result.x = v1.y * v2.z - v1.z * v2.y;
		result.y = v1.z * v2.x - v1.x * v2.z;
		result.z = v1.x * v2.y - v1.y * v2.x;
		return result;
	}


	// static const int kRowHeight = 30;
	// static const int kColumnWidth = 60;

	// void VectorScrenPrintf(int x, int y, const Vector3& vector, const char* label) {
	//	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	//	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	//	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	//	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
	// }
};
