#pragma once
#include "Model.h"
#include "WorldTransform.h"

class MapChipField;

class Enemy {

public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Updata();
	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

private:
	WorldTransform worldTransform_;

	ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	//補講の速さ
	static inline const float kWalkSpeed = 0.02f;
	//速度
	Vector3 velocity_ = {};

	//最初の角度[ 度 ]
	static inline const float kWalkMotionAngleStart = 30.0f;
	//最後の角度[ 度 ]
	static inline const float kWalkMotionAngleEnd = -60.0f;
	//アニメーションの周期となる時間[ 秒 ]
	static inline const float kWalkMotionTime = 2.0f;

	//経過時間
	float walkTimer_ = 0.0f;
};
