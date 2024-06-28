#include "Enemy.h"
#include "Vector3.h"
#include "cassert"
#define _USE_MATH_DEFINES
#include "ImGuiManager.h"
#include <cmath>
#include <math.h>

// Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
//	if (t <= 0.0f) {
//		return a;
//	} else if (t >= 1.0f) {
//		return b;
//	}
//
//	float x = a.x + t * (b.x - a.x);
//	float y = a.y + t * (b.y - a.y);
//	float z = a.z + t * (b.z - a.z);
//
//	return {x, y, z};
// }

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = -1.5f;

	viewProjection_ = viewProjection;

	// 速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};
	// 経過時間の設定
	walkTimer_ = 0.0f;
}

void Enemy::Updata() {

	// 移動
	worldTransform_.translation_ += velocity_;

	// タイマーを加算
	walkTimer_ += 1.0f / 60.0f;
	// アニメーションの進行度を計算
	float animationProgress = walkTimer_ / kWalkMotionTime;
	// 回転アニメーション
	float param =float( std::sin(animationProgress * 2 * M_PI));
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = (radian) * float(M_PI / 180.0f); // 度をラジアンに変換
	// ワールド行列の更新
	worldTransform_.UpdateMatrix();

	ImGui::Begin("A");
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 3.0f);
	ImGui::DragFloat("param", &param,1.0f);
	ImGui::DragFloat("radian", &radian, 1.0f);
	ImGui::End();
}

void Enemy::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
