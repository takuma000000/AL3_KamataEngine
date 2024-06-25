﻿#define NOMINMAX
#include "Player.h"
#include "ImGuiManager.h"
#include "cassert"
#include <Input.h>
#include <algorithm>
#include <numbers>
#include <MapChipField.h>
#include <DebugText.h>
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
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 0.5f;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	viewProjection_ = viewProjection;
}

void Player::Update() {

	worldTransform_.UpdateMatrix();

	worldTransform_.translation_ += velocity_;

	// 移動入力
	KeyMove();

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.isMovement = velocity_;

	// マップ衝突チェック
	HitMap(collisionMapInfo);

	//旋回制御
	TurningControl();

	

	worldTransform_.UpdateMatrix();

	// 着地フラグ
	bool landing = false;

	// 地面との当たり判定
	if (velocity_.y < 0) {
		// y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	// 接地判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態に移動
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation_);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}

	//
	ImGui::Begin("A");
	ImGui::SliderFloat3("velocity", &velocity_.x, 0.0f, 1.0f);
	ImGui::End();
}

void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

void Player::KeyMove() {

	// 移動入力
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
		// 空中
	} else {
		velocity_ += Vector3(0, kGravityAcc, 0);
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}

void Player::TurningControl() {

	//旋回制御
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

}

void Player::HitMap(CollisionMapInfo& info) {

	HitMapUp(info);
	/*HitMapDown(info);
	HitMapRight(info);
	HitMapLeft(info);*/

}

void Player::HitMapUp(CollisionMapInfo& info) {

	// 上昇あり？
	 if (info.isMovement.y <= 0) {
		return;
	 }

	//移動後の4つの角の座標
	 std::array<Vector3, kNumCorner> positionsNew;

	 for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] =
		    CornersPosition(worldTransform_.translation_ + info.isMovement, static_cast<Corner>(i));
	 }

	 MapChipType mapChipType;
	 //真上の当たり判定を行う
	 bool hit = false;

	 //左上点の判定
	 MapChipField::IndexSet indexSet;
	 indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	 mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	 if (mapChipType == MapChipType::kBlock) {
		hit = true;
	 }

	 //右上点の判定
	 MapChipField::IndexSet indexSet2;
	 indexSet2 = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	 mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet2.xIndex, indexSet2.yIndex);
	 if (mapChipType == MapChipType::kBlock) {
		hit = true;
	 }

	 //ブロックにヒット
	 if (hit) {
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(
		    worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));

		//めり込み先ブロックの範囲矩形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.isMovement.y = std::max(
		    0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));

		// 衝突判定
		info.isCeilingHit = true;
	 }

	 //衝突
	 CeilingContact(info);

}

Vector3 Player::CornersPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {

	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},//kRightBottom

	    {-kWidth / 2.0f, -kHeight / 2.0f, 0},//kLeftBottom

	    {+kWidth / 2.0f, +kHeight / 2.0f, 0},//kRightTop

	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}//kLeftTop
	};
	
	return center + offsetTable[static_cast<uint32_t>(corner)];

}

void Player::ReflectionResult(const CollisionMapInfo& info) {
	//移動
	worldTransform_.translation_ += info.isMovement;
}

void Player::CeilingContact(const CollisionMapInfo& info) {
	//天井に当たった？
	if (info.isCeilingHit) {
		DebugText::GetInstance()->ConsolePrintf("hit celing\n");
		velocity_.y = 0;
	}
}
