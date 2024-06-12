#include "CameraController.h"
#include "Player.h"


void CameraController::Initialize(const ViewProjection& viewProjection) {

	 viewProjection_ = viewProjection;

}

void CameraController::Update() {}

void CameraController::Reset() {

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffest_;

}
