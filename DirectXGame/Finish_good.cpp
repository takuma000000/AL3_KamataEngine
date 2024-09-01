#include "Finish_good.h"
#include <cassert>

void Finish_good::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);

	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
}

void Finish_good::Update() {}

void Finish_good::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}
