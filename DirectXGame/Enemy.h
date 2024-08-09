#pragma once
#include "Model.h"
#include "WorldTransform.h"

enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

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

	// フェーズ
	Phase phase_ = Phase::Approach; // 初期フェーズを設定
};
