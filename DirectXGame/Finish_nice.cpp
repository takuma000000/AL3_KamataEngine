#include "Finish_nice.h"
#include <cassert>

void Finish_nice::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);

	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
}

void Finish_nice::Update() {}

void Finish_nice::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}
