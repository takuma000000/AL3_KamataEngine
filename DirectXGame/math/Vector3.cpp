#include "Vector3.h"

// スカラーとの乗算のための非メンバ関数の定義
Vector3 operator*(const float scalar, const Vector3& vector) {
	return Vector3{vector.x * scalar, vector.y * scalar, vector.z * scalar};
}