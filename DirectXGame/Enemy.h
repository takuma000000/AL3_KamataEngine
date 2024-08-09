#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Enemy {

public:

	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);

	void Update();

	void Draw(const ViewProjection& viewProjection);

private:

	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

};
