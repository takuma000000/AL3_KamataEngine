//#include "WorldTransform.h"
//#include "GameScene.h"
//#include <cassert>
//#include <cmath>
//
////class WorldTransform;
//Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
//	Matrix4x4 result;
//
//	// 単位行列に設定します
//	for (int i = 0; i < 4; ++i) {
//		for (int j = 0; j < 4; ++j) {
//			result.m[i][j] = (i == j) ? 1.0f : 0.0f;
//		}
//	}
//
//	// 拡大成分を設定します
//	result.m[0][0] = scale.x;
//	result.m[1][1] = scale.y;
//	result.m[2][2] = scale.z;
//
//	return result;
//}
//
//Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
//	Vector3 result;
//
//	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
//	           matrix.m[3][0];
//	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
//	           matrix.m[3][1];
//	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
//	           matrix.m[3][2];
//	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
//	          1.0f * matrix.m[3][3];
//	assert(w != 0.0f);
//	result.x /= w;
//	result.y /= w;
//	result.z /= w;
//
//	return result;
//}
//
//Matrix4x4 MakeRotateXMatrix(float radian) {
//	float c = std::cos(radian);
//	float s = std::sin(radian);
//
//	Matrix4x4 rotationMatrix;
//
//	rotationMatrix.m[0][0] = 1.0f;
//	rotationMatrix.m[0][1] = 0.0f;
//	rotationMatrix.m[0][2] = 0.0f;
//	rotationMatrix.m[0][3] = 0.0f;
//	rotationMatrix.m[1][0] = 0.0f;
//	rotationMatrix.m[1][1] = c;
//	rotationMatrix.m[1][2] = s;
//	rotationMatrix.m[1][3] = 0.0f;
//	rotationMatrix.m[2][0] = 0.0f;
//	rotationMatrix.m[2][1] = -s;
//	rotationMatrix.m[2][2] = c;
//	rotationMatrix.m[2][3] = 0.0f;
//	rotationMatrix.m[3][0] = 0.0f;
//	rotationMatrix.m[3][1] = 0.0f;
//	rotationMatrix.m[3][2] = 0.0f;
//	rotationMatrix.m[3][3] = 1.0f;
//
//	return rotationMatrix;
//}
//
//Matrix4x4 MakeRotateYMatrix(float radian) {
//	float c = std::cos(radian);
//	float s = std::sin(radian);
//
//	Matrix4x4 rotationMatrix;
//
//	rotationMatrix.m[0][0] = c;
//	rotationMatrix.m[0][1] = 0.0f;
//	rotationMatrix.m[0][2] = -s;
//	rotationMatrix.m[0][3] = 0.0f;
//	rotationMatrix.m[1][0] = 0.0f;
//	rotationMatrix.m[1][1] = 1.0f;
//	rotationMatrix.m[1][2] = 0.0f;
//	rotationMatrix.m[1][3] = 0.0f;
//	rotationMatrix.m[2][0] = s;
//	rotationMatrix.m[2][1] = 0.0f;
//	rotationMatrix.m[2][2] = c;
//	rotationMatrix.m[2][3] = 0.0f;
//	rotationMatrix.m[3][0] = 0.0f;
//	rotationMatrix.m[3][1] = 0.0f;
//	rotationMatrix.m[3][2] = 0.0f;
//	rotationMatrix.m[3][3] = 1.0f;
//
//	return rotationMatrix;
//}
//
//Matrix4x4 MakeRotateZMatrix(float radian) {
//	float c = std::cos(radian);
//	float s = std::sin(radian);
//
//	Matrix4x4 rotationMatrix;
//
//	rotationMatrix.m[0][0] = c;
//	rotationMatrix.m[0][1] = s;
//	rotationMatrix.m[0][2] = 0.0f;
//	rotationMatrix.m[0][3] = 0.0f;
//	rotationMatrix.m[1][0] = -s;
//	rotationMatrix.m[1][1] = c;
//	rotationMatrix.m[1][2] = 0.0f;
//	rotationMatrix.m[1][3] = 0.0f;
//	rotationMatrix.m[2][0] = 0.0f;
//	rotationMatrix.m[2][1] = 0.0f;
//	rotationMatrix.m[2][2] = 1.0f;
//	rotationMatrix.m[2][3] = 0.0f;
//	rotationMatrix.m[3][0] = 0.0f;
//	rotationMatrix.m[3][1] = 0.0f;
//	rotationMatrix.m[3][2] = 0.0f;
//	rotationMatrix.m[3][3] = 1.0f;
//
//	return rotationMatrix;
//}
//
//Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
//	Matrix4x4 result;
//
//	// 単位行列に設定します
//	for (int i = 0; i < 4; ++i) {
//		for (int j = 0; j < 4; ++j) {
//			if (i == j) {
//				result.m[i][j] = 1.0f;
//			} else {
//				result.m[i][j] = 0.0f;
//			}
//		}
//	}
//
//	// 移動成分を設定します
//	result.m[3][0] = translate.x;
//	result.m[3][1] = translate.y;
//	result.m[3][2] = translate.z;
//
//	return result;
//}
//
//Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
//	Matrix4x4 result;
//
//	for (int i = 0; i < 4; ++i) {
//		for (int j = 0; j < 4; ++j) {
//			result.m[i][j] = 0;
//			for (int k = 0; k < 4; ++k) {
//				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
//			}
//		}
//	}
//
//	return result;
//}
//
//Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate) {
//	Matrix4x4 affineMatrix;
//
//	// 各変換行列を作成
//	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
//
//	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
//	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
//	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
//	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
//
//	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
//
//	// 各変換行列を合成してアフィン変換行列を作成
//	affineMatrix = Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
//
//	return affineMatrix;
//}
//
//void WorldTransform::UpdateMatrix() {
//
//	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
//
//	TransferMatrix();
//
//}