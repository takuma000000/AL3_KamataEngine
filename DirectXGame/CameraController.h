#pragma once
#include "Model.h"
#include "Vector3.h"
#include "WorldTransform.h"
// #include <Player.h>

// 矩形
struct Rect {
	float left = 0.0f;
	float right = 1.0f;
	float bottom = 0.0f;
	float top = 1.0f;
};

// 前方宣言
class Player;

class CameraController {

	

public:

	void Initialize();

	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	const Matrix4x4 GetView() { return viewProjection_.matView; }
	const Matrix4x4 GetProjection() { return viewProjection_.matProjection; }

	void SetMovableArea(Rect area) { movableArea_ = area; }

	//座標保管割合
	static inline const float kInterpolationRate = 0.1f;

	//速度掛け率
	static inline const float kVelocityBias = 30.0f;

	//追従対象の各方向へのカメラ移動範囲
	static inline const Rect margin = {-100, 100, -60, 60};

private:

	ViewProjection viewProjection_;

	Player* target_ = nullptr;

	Vector3 targetOffset_ = {0.0f, 0.0f, -20.0f};

	// カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};

	//カメラの目標座標
	Vector3 mark_;

};
