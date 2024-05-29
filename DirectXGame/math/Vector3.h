#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

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
};