#include "PlayerBullet.h"
#include "cassert"
#include "MyMath.h"
#include "Vector3.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("uvChecker.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標をリセット
	worldTransform_.translation_ = {position.x, position.y, position.z};

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = Vector3{velocity.x, velocity.y, velocity.z};
}

void PlayerBullet::Update() {
	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//座標を移動させる( 1フレーム分の移動量を足しこむ )
	worldTransform_.translation_ += velocity_;

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollision() { isDead_ = true; }

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void PlayerBullet::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}
