#include "Enemy.h"
#include "MyMath.h"
#include "cassert"

void Enemy::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();

	//Y座標を指定
	worldTransform_.translation_.y = 5.0f;
	//Z座標を指定
	worldTransform_.translation_.z = 10.0f;
}

void Enemy::Update() {

	//接近速度
	Vector3 approachVelocity = {0.0f, 0.0f, -0.1f};
	//離脱速度
	Vector3 leaveVelocity = {-0.1f, -0.1f, 0.0f};

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//switch文による実装
	switch (phase_) { 
	case Phase::Approach:
	default:
		//移動( ベクトルを加算 )
		worldTransform_.translation_ += approachVelocity;
		//既定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		//移動( ベクトルを加算 )
		worldTransform_.translation_ += leaveVelocity;
		break;
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
