#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <Player.h>

// 前方宣言
class Player;

class CameraController {


public:

	void Initialize();

	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

private:

	ViewProjection viewProjection_;

	Player* target_ = nullptr;

	Vector3 targetOffest_ = {0, 0, -15.0f};

	WorldTransform worldTransform_;

};


