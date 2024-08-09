#include "Enemy.h"
#include "MyMath.h"
#include "cassert"
#include "ImGuiManager.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();

	//X座標を指定
	worldTransform_.translation_.x = 30.0f;
	//Y座標を指定
	worldTransform_.translation_.y = 5.0f;
	//Z座標を指定
	worldTransform_.translation_.z = 10.0f;

	// 弾発射
	//Fire();

	//接近フェーズ初期化
	ApproachInitialize();
}

void Enemy::Update() {

	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//接近速度
	Vector3 approachVelocity = {0.0f, 0.0f, -0.05f};
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

	// 弾の更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	// 接近フェーズ更新
	ApproachUpdate();

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾の描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire() {
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = MyMath::TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);
}

Enemy::Enemy() {}

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::ApproachInitialize() {
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}

void Enemy::ApproachUpdate() {
	//発射タイマーをカウントダウン
	fireTimer--;
	//指定時間に達した
	if (fireTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = kFireInterval;
	}
}
