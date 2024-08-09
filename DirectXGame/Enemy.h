#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include <list>
#include "Input.h"

enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

class Enemy {

public:
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	//弾発射
	void Fire();

	Enemy();
	~Enemy();

	//発射間隔
	static const int kFireInterval = 60;

	//接近フェーズ初期化
	void ApproachInitialize();
	//接近フェーズ更新
	void ApproachUpdate();

private:
	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	// フェーズ
	Phase phase_ = Phase::Approach; // 初期フェーズを設定

	// 弾
	std::list<EnemyBullet*> bullets_;
	
	//発射タイマー
	int32_t fireTimer = 0;
};
