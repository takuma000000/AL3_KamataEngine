#pragma once
#include "EnemyBullet.h"
#include "Input.h"
#include "Model.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include <list>

// 自機クラスの前方処理
class Player;

// GameSceneの前方宣言
class GameScene;

enum class Phase {
	Approach, // 接近する
	Leave,    // 離脱する
};

class Enemy {

public:
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection,Vector3 position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	// 弾発射
	void Fire();

	Enemy();
	~Enemy();

	// 発射間隔
	static const int kFireInterval = 60;

	// 接近フェーズ初期化
	void ApproachInitialize();
	// 接近フェーズ更新
	void ApproachUpdate();

	void SetPlayer(Player* player) { player_ = player; }

	// ポジションをポインタでセットするメソッド
	void SetPos(Vector3* position) { position_ = position; }

	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	// デスフラグ
	bool isDead() const { return isDead_; }

private:
	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	// フェーズ
	Phase phase_ = Phase::Approach; // 初期フェーズを設定

	// 発射タイマー
	int32_t fireTimer = 0;

	// 自キャラ
	Player* player_ = nullptr;

	Vector3* position_ = nullptr; // ポインタ型として宣言

	// GameScene
	GameScene* gameScene_ = nullptr;

	// デスフラグ
	bool isDead_ = false;
};
