#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Finish_good {
public:
	void Initialize(Model* model, ViewProjection* viewProjection);
	void Update();
	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	// モデル
	Model* model_ = nullptr;
};
