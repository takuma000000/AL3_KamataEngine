#include "DeathParticles.h"
#include "WorldTransformEX.cpp"
#include "cassert"

void DeathParticles::Initialize(
    Model* model, ViewProjection* viewProjection, const Vector3& position) {

	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	for (auto& worldTransform : worldTransform_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
}

void DeathParticles::Update() {

	// ワールド変換の更新
	for (auto& worldTransform : worldTransform_) {
		// MakeAffineMatrix 	VRAMに転送
		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw() {
	// ワールド変換の描画処理
	for (auto& worldTransform : worldTransform_) {
		model_->Draw(worldTransform, *viewProjection_);
	}
}
