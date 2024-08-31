#include "Finish.h"
#include <cassert>

void Finish::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);

	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
}

void Finish::Update() {}

void Finish::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}
