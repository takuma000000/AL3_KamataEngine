#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "array"

class DeathParticles {

public:

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

private:

	ViewProjection* viewProjection_ = nullptr;

	Model* model_ = nullptr;

	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransform_;

};
