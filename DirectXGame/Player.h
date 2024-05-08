﻿#pragma once

#include "WorldTransform.h"
#include "Model.h"


class Player {

public:

	void Initialize(Model *model,uint32_t textureHandle);

	void Update();

	void Draw();

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

};
