#include "Player.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "WinApp.h"
#include "cassert"

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 vector) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.translation_ = vector;

	// レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("point.png");
	// 色
	Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	// アンカーポイント
	Vector2 point = {0.5f, 0.5f};
	// スプライト生成
	sprite2D_ = Sprite::Create(textureReticle, Vector2(worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y), color, point);

	// 初期化
	worldTransform_.Initialize();
	worldTransform3DReticle_.Initialize();

	// シングルトンインスタンス
	input_ = Input::GetInstance();
}

void Player::Update(const ViewProjection& viewProjection) {

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

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

	// キャラクター攻撃処理
	Attack();
	// 弾の更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//// 自機のワールド座標から3Dレティクルのワールド座標を計算
	const float kDistance = 50.0f; // 自機からの距離
	Vector3 offset = {0, 0, 1.0f}; // Z+方向のオフセット
	offset = MyMath::Multiply(offset, worldTransform_.matWorld_);
	offset = MyMath::Normalize(offset) * kDistance;
	worldTransform3DReticle_.translation_ = worldTransform_.translation_ + offset;
	worldTransform3DReticle_.UpdateMatrix();

	// 3Dレクティルのワールド行列から2Dレクティルのスクリーン座標を計算
	Vector3 posReticle;
	posReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
	posReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
	posReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];

	// ビューポート行列
	Matrix4x4 matViewPort =
	    MyMath::MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列を使って計算する部分
	Matrix4x4 matViewProjectionViewPort =
	    viewProjection.matView * viewProjection.matProjection * matViewPort;
	// ワールド->スクリーン座標変換( ここで3Dから2Dになる )
	posReticle = MyMath::Transform(posReticle, matViewProjectionViewPort);
	// スプライトのレティクル座標に設定
	sprite2D_->SetPosition(Vector2(posReticle.x, posReticle.y));
	sprite2D_->SetSize(Vector2(150.0f, 150.0f));

	// キャラクターの座標を表示する処理
	ImGui::Begin("chara");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat2("translation2D", &posReticle.x, 0.01f);
	ImGui::DragFloat3("rotate3D", &worldTransform3DReticle_.rotation_.x, 0.01f);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// 3Dレティクルを描画
	//model_->Draw(worldTransform3DReticle_, viewProjection);

	// DrawUI();

	// 弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
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
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	// 定数バッファに転送する
	worldTransform_.TransferMatrix();
}

void Player::Attack() {
	// Zキーをトリガーしたら発射
	if (input_->PushKey(DIK_Z)) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;

		// 3Dレティクルのワールド座標と自機のワールド座標の差を計算
		Vector3 velocity = worldTransform3DReticle_.translation_ - worldTransform_.translation_;

		// ベクトルの正規化と弾の速さを掛け合わせる
		velocity = MyMath::Normalize(velocity) * kBulletSpeed;

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);
		newBullet->SetParent(worldTransform_.parent_);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

Player::Player() {}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2D_;
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得( ワールド座標 )
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {}

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

void Player::DrawUI() { sprite2D_->Draw(); }
