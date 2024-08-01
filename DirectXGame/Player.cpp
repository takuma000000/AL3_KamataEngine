#include "Player.h"
#include "cassert"


void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();

	// シングルトンインスタンス
	input_ = Input::GetInstance();
}

void Player::Update() {

	worldTransform_.TransferMatrix();

	//キャラの移動ベクトル
	Vector3 move = {0, 0, 0};
	//キャラの移動速度
	const float kCharaSpeed = 0.2f;

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharaSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharaSpeed;
	}
	//移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y -= kCharaSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y += kCharaSpeed;
	}

	//座標移動
	worldTransform_.translation_ += move;

	
}

void Player::Draw() {

	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);

}
