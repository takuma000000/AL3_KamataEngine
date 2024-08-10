#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class RailCamera {

public:
	void Initialize(WorldTransform& transform, ViewProjection* viewProjection);
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() const {
		return *viewProjection_; // viewProjection_ をデリファレンスして返す
	}

	// ゲッター
	const WorldTransform* GetWorldTransform() const { return worldTransform_; }

private:
	// ワールド変換データ
	WorldTransform* worldTransform_;
	// ビュープロジェクション
	ViewProjection* viewProjection_;
};
