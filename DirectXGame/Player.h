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

	//接地招待フラグ
	bool onGround_ = true;

	//重力加速度　　下方向
	static inline const float kGravityAcc = -0.05f;
	//最大落下速度　　下方向
	static inline const float kLimitFallSpeed = 1.0f;
	//ジャンプ初速　　上方向
	static inline const float kJumpAcc = 0.5f;

	

};
