#include "Enemy.h"
#include "MyMath.h"
#include "cassert"

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	// NULLポインタチェック
	assert(model);
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemy.jpg");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をリセット
	worldTransform_.translation_ = {position.x, position.y, position.z};

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = Vector3{velocity.x, velocity.y, velocity.z};

}

void Enemy::Update() {

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
