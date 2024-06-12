#include "CameraController.h"
#include "Player.h"


void CameraController::Initialize() {

	 viewProjection_.Initialize();

}

void CameraController::Update() {}

void CameraController::Reset() {

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffest_;

}
