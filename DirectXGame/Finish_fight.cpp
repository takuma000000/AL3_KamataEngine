#include "Finish_fight.h"
#include <cassert>

void Finish_fight::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);

	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
}

void Finish_fight::Update() {}

void Finish_fight::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}

