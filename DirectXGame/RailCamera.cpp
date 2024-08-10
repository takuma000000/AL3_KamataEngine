#include "RailCamera.h"
#include "ImGuiManager.h"
#include "MyMath.h"

void RailCamera::Initialize(WorldTransform& transform, ViewProjection* viewProjection) {
	// ワールドトランスフォームの初期化
	worldTransform_ = &transform;

	// ここでワールド行列を再計算することも検討
	worldTransform_->UpdateMatrix();

	// ビュープロジェクションの初期化
	if (viewProjection != nullptr) {
		viewProjection_ = viewProjection;
		viewProjection_->Initialize(); // 初期化メソッドがある場合
	}
}

void RailCamera::Update() {
	// ワールドトランスフォームの座標に加算 (例: カメラがZ軸方向に移動)
	worldTransform_->translation_.z -= 0.1f; // Z軸方向に0.1単位移動

	//// ワールドトランスフォームの角度に加算 (例: Y軸回りに回転)
	// float degree = 1.0f;                             // 1度
	// float radians = degree * (3.14f / 180.0f); // ラジアンに変換
	// worldTransform_.rotation_.y += radians;          // Y軸回りにラジアン分回転

	// ワールドトランスフォームのワールド行列を再計算
	worldTransform_->UpdateMatrix();

	// ビュー行列を設定
	if (viewProjection_ != nullptr) {
		viewProjection_->matView = MyMath::Inverse4x4(worldTransform_->matWorld_);
	}

	// ImGui を使用してカメラの座標を表示
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translation", &worldTransform_->translation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotation", &worldTransform_->rotation_.x, -10.0f, 10.0f);
	ImGui::End();
}