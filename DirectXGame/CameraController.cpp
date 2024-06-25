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


	const Vector3& targetVelocity = target_->GetVelocity();


	//追従対象とオフセットと追従対象の速度からカメラの目標座標を計算
	mark_ = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBias;

	//座標補間によりゆったり追従
	viewProjection_.translation_ = Lerp(viewProjection_.translation_, mark_, kInterpolationRate);

	//追従対象が画面外に出ないように補正
	viewProjection_.translation_.x =
	    std::max(viewProjection_.translation_.x, targetWorldTransform.translation_.x + margin.left);
	viewProjection_.translation_.x =
	    std::min(viewProjection_.translation_.x, targetWorldTransform.translation_.x + margin.right);
	viewProjection_.translation_.y =
	    std::max(viewProjection_.translation_.y, targetWorldTransform.translation_.y + margin.bottom);
	viewProjection_.translation_.y = std::min(
	    viewProjection_.translation_.y, targetWorldTransform.translation_.y + margin.top);

	//移動範囲制限
	viewProjection_.translation_.x = std::min(viewProjection_.translation_.x, movableArea_.left);
	viewProjection_.translation_.x = std::max(viewProjection_.translation_.x, movableArea_.right);
	viewProjection_.translation_.y = std::min(viewProjection_.translation_.y, movableArea_.bottom);
	viewProjection_.translation_.y = std::max(viewProjection_.translation_.y, movableArea_.top);

	//行列を更新する
	viewProjection_.UpdateMatrix();

	

}

void CameraController::Reset() {

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	viewProjection_.translation_ = targetWorldTransform.translation_ + targetOffset_;

}
