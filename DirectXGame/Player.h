#pragma once
#include "Model.h"
#include "WorldTransform.h"

struct CollisionMapInfo {
	//天井衝突
	bool isCeilingHit = false;
	//着地
	bool isLanding = false;
	//壁接触
	bool isWallHit = false;
	//移動量
	Vector3 isMovement;
};

enum class LRDirection { 
	kRight,
	kLeft,
};

enum Corner {
	kRightBottom,//右下
	kLeftBottom,//左下
	kRightTop,//右上
	kLeftTop,//左上

	kNumCorner//要素数
};

class MapChipField;

class Player {

public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void KeyMove();

	//マップ衝突判定
	void HitMap(CollisionMapInfo& info);

	//指定した門野座標計算
	Vector3 CornersPosition(const Vector3& center, Corner corner);

private:
	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Vector3 velocity_ = {};

	static inline const float kAcceleratuon_ = 0.1f;
	static inline const float kAttenuation_ = 0.1f;
	static inline const float kLimitRunSpeed = 0.25f;

	LRDirection lrDirection_ = LRDirection::kRight;

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;

	static inline const float kTimeTurn = 0.3f;


	//接地招待フラグ
	bool onGround_ = true;

	//重力加速度　　下方向
	static inline const float kGravityAcc = -0.05f;
	//最大落下速度　　下方向
	static inline const float kLimitFallSpeed = 1.0f;
	//ジャンプ初速　　上方向
	static inline const float kJumpAcc = 0.5f;

	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	//キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

};
