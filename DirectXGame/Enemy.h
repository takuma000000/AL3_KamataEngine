#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Enemy {

public:

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

private:

	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	// ‘¬“x
	Vector3 velocity_;

};
