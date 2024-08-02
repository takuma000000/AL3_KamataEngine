#include "Player.h"
#include "ImGuiManager.h"
#include "MyMath.h"
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

	// キャラの移動ベクトル
	Vector3 move = {0, 0, 0};
	// キャラの移動速度
	const float kCharaSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharaSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharaSpeed;
	}
	// 移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharaSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharaSpeed;
	}

	// 移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 20.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 座標移動
	worldTransform_.translation_ += move;
	worldTransform_.UpdateMatrix();

	//キャラクター攻撃処理
	Attack();
	//弾の更新
	if (bullet_) {
		bullet_->Update();
	}

	// キャラクターの座標を表示する処理
	ImGui::Begin("chara");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_); 

	//弾の描画
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
}

void Player::Rotate() {
	// 回転速さ[ ラジアン/frame ]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::UpdateMatrix() {
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 定数バッファに転送する
	worldTransform_.TransferMatrix();
}

void Player::Attack() {
	//Zキーをトリガーしたら発射
	if (input_->PushKey(DIK_Z)) {
		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		//弾を登録する
		bullet_ = newBullet;
	}
}
