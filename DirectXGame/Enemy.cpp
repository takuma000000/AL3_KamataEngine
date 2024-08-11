#include "Enemy.h"
#include "MyMath.h"
#include "cassert"
#include "ImGuiManager.h"
#include "Player.h"
#include "GameScene.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection,Vector3 position) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	worldTransform_.translation_ = position;
	worldTransform_.Initialize();

	////X座標を指定
	//worldTransform_.translation_.x = 30.0f;
	////Y座標を指定
	//worldTransform_.translation_.y = 5.0f;
	////Z座標を指定
	//worldTransform_.translation_.z = 20.0f;

	// 弾発射
	//Fire();

	//接近フェーズ初期化
	ApproachInitialize();
}

void Enemy::Update() {

	// 接近フェーズ更新
	ApproachUpdate();

	//接近速度
	Vector3 approachVelocity = {0.0f, 0.0f, -0.01f};
	//離脱速度
	Vector3 leaveVelocity = {0.0f, 0.0f, -0.01f};

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

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Fire() {
	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 1.0f;
	//Vector3 velocity(0, 0, -kBulletSpeed);

	//自キャラのワールド座標を取得する
	Vector3 playerWorldPos = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得する
	Vector3 enemyWorldPos = GetWorldPosition();
	//敵キャラから自キャラへの差分ベクトルを求める
	Vector3 diff = playerWorldPos - enemyWorldPos;
	//ベクトルの正規化
	Vector3 direction = MyMath::Normalize(diff);
	//ベクトルの長さを、速さに合わせる
	Vector3 velocity = direction * kBulletSpeed;

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = MyMath::TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
}

Enemy::Enemy() {}

Enemy::~Enemy() {
	
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

Vector3 Enemy::GetWorldPosition() { 
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得( ワールド座標 )
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() { isDead_ = true; }
