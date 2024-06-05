#define NOMINMAX
#include "Player.h"
#include "cassert"
#include <Input.h>
#include <algorithm>
#include <numbers>
// #include "Vector3.h"

float EaseInOutQuad(float t) {
	if (t < 0.5f) {
		return 2.0f * t * t;
	} else {
		return -1.0f + (4.0f - 2.0f * t) * t;
	}
}

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {

	assert(model);

	model_ = model;

	// textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	viewProjection_ = viewProjection;
}

void Player::Update() {

	worldTransform_.UpdateMatrix();

	
	if (onGround_) {

		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acc = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation_);
				}
				acc.x += kAcceleratuon_;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;

					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation_);
				}
				acc.x -= kAcceleratuon_;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;

					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}
			velocity_ += acc;
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			velocity_.x *= (1.0f - kAttenuation_);

			if (Input::GetInstance()->PushKey(DIK_UP)) {
				// ジャンプ初速
				velocity_ += Vector3(0, kJumpAcc, 0);
			}

		}
	//空中
	} else {
		velocity_ += Vector3(0, -kGravityAcc, 0);
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	worldTransform_.translation_ += velocity_;

	if (turnTimer_ > 0.0f) {

		turnTimer_ -= kTimeTurn / 60.0f;

		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 現在の角度を旋回開始時の角度から目標の角度に補完する
		float t = 1.0f - (turnTimer_ / kTimeTurn);
		float currentRotationY =
		    turnFirstRotationY_ + (destinationRotationY - turnFirstRotationY_) * t;
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = currentRotationY;
	}

	worldTransform_.UpdateMatrix();

	//着地フラグ
	bool landing = false;

	//地面との当たり判定
	if (velocity_.y < 0) {
		//y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	//接地判定
	if (onGround_) {
		//ジャンプ開始
		if (velocity_.y > 0.0f) {
			//空中状態に移動
			onGround_ = false;
		}
	} else {
		//着地
		if (landing) {
			//めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			//摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation_);
			//下方向速度をリセット
			velocity_.y = 0.0f;
			//接地状態に移行
			onGround_ = true;
		}
	}

}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
