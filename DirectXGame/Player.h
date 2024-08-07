#pragma once
#include "Input.h"
#include "WorldTransform.h"
#include "Model.h"
#include "PlayerBullet.h"

#include <memory>

class Player {

public:

	void Initialize(Model *model,uint32_t textureHandle,ViewProjection* viewProjection);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Rotate();

	void UpdateMatrix();

	//攻撃
	void Attack();


	Player();
	~Player();

private:

	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;

	//弾
	PlayerBullet* bullet_ = nullptr;
	
};
