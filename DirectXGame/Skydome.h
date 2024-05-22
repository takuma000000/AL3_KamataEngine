#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Skydome {

public:

	void Initiaize(Model* model,ViewProjection* viewProjection);

	void Update();

	void Draw();


private:

	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;

	Model* model_ = nullptr;

};
