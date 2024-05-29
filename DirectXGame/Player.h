#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Player {

public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

private:
	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Vector3 velocity_ = {};

	static inline const float kAcceleratuon_ = 1.0f;
	static inline const float kAttenuation_ = 0.5f;
	static inline const float kLimitRunSpeed = 1.0f;
};
