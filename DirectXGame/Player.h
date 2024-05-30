#pragma once
#include "Model.h"
#include "WorldTransform.h"


enum class LRDirection { 
	kRight,
	kLeft,
};


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

	static inline const float kAcceleratuon_ = 0.1f;
	static inline const float kAttenuation_ = 0.1f;
	static inline const float kLimitRunSpeed = 0.25f;

	LRDirection lrDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;

	static inline const float kTimeTurn = 0.3f;

};
