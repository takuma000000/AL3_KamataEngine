#define NOMINMAX
#include <algorithm>
#include "CameraController.h"
#include "Player.h"
#include <iostream>

Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
	if (t <= 0.0f) {
		return a;
	} else if (t >= 1.0f) {
		return b;
	}

	float x = a.x + t * (b.x - a.x);
	float y = a.y + t * (b.y - a.y);
	float z = a.z + t * (b.z - a.z);

	return {x, y, z};
}

void CameraController::Initialize() {

	 viewProjection_.Initialize();

	

}

void CameraController::Update() {

	//追従対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	//追従対象とオフセットからカメラの目標座標を計算
	mark_ = targetWorldTransform.translation_ + targetOffset_;

	//座標補間二よりゆったり追従
	viewProjection_.translation_ = Lerp(viewProjection_.translation_, mark_, kInterpolationRate);

	//移動範囲制限
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, movableArea_.left);
	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, movableArea_.right);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, movableArea_.bottom);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, movableArea_.top);

	viewProjection_.UpdateMatrix();

}

void CameraController::Reset() {

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffset_;

}
