#include "Enemy.h"
#include "MyMath.h"
#include "cassert"

void Enemy::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();

}

void Enemy::Update() {

	//速度を決めて加算
	Vector3 velocity = {0.0f, 0.0f, -0.1f};
	worldTransform_.translation_ += velocity;

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画	
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
