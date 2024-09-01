#include "Setumei.h"
#include <cassert>

void Setumei::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);

	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
}

void Setumei::Update() {}

void Setumei::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}

