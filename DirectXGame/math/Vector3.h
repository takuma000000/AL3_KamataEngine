#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	 // コンストラクタ
	Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

	// += 演算子のオーバーロード
	Vector3& operator+=(const Vector3& other) {
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}

	// + 演算子のオーバーロード (便利のために追加)
	Vector3 operator+(const Vector3& other) const {
		return Vector3{this->x + other.x, this->y + other.y, this->z + other.z};
	}

	// -= 演算子のオーバーロード (必要に応じて追加)
	Vector3& operator-=(const Vector3& other) {
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		return *this;
	}

	// - 演算子のオーバーロード (便利のために追加)
	Vector3 operator-(const Vector3& other) const {
		return Vector3{this->x - other.x, this->y - other.y, this->z - other.z};
	}

	// *= 演算子のオーバーロード (スカラーとの乗算)
	Vector3& operator*=(const float scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		return *this;
	}

	// * 演算子のオーバーロード (スカラーとの乗算)
	Vector3 operator*(const float scalar) const {
		return Vector3{this->x * scalar, this->y * scalar, this->z * scalar};
	}

	// *= 演算子のオーバーロード (要素ごとの乗算)
	Vector3& operator*=(const Vector3& other) {
		this->x *= other.x;
		this->y *= other.y;
		this->z *= other.z;
		return *this;
	}

	// * 演算子のオーバーロード (要素ごとの乗算)
	Vector3 operator*(const Vector3& other) const {
		return Vector3{this->x * other.x, this->y * other.y, this->z * other.z};
	}
};

// スカラーとの乗算のための非メンバ関数の宣言
Vector3 operator*(const float scalar, const Vector3& vector);
