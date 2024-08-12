#pragma once
#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "WorldTransform.h"
#include <Sprite.h>
#include <list>
#include <memory>

class Player {

public:
	void Initialize(Model* model, uint32_t textureHandle, Vector3 vector);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Rotate();

	void UpdateMatrix();

	// 攻撃
	void Attack();

	Player();
	~Player();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	// 親となるワールドトランスフォームをセット
	void SetParent(const WorldTransform* parent);

	// UI描画
	void DrawUI();

private:
	WorldTransform worldTransform_;

	// ViewProjectionのポインタを追加
	ViewProjection viewProjection_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	// キーボード入力
	Input* input_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;

	//	3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	// 2Dレティクル用スプライト
	Sprite* sprite2D_ = nullptr;
};
