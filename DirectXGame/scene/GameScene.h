#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "RailCamera.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "skydome.h"
#include <list>
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// 衝突判定と応答
	void CheckAllCollision();

	// 弾リストを取得
	// const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	// 敵弾を追加する
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	// 敵発生データの読み込み
	void LoadEnemyPopData();

	// 敵発生コマンドの更新
	void UpdateEnemyPopCommands();

	void EnemyPop(Vector3 position);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t textureHandle_ = 0;

	Model* model_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;

	Model* modelPlayer_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	// 速度
	Vector3 velocity_;
	uint32_t enemyTextureHandle_ = 0;

	// 天球
	skydome* skydome_ = nullptr;

	// 3Dモデル 天球
	Model* modelSkydome_ = nullptr;

	// RailCamera
	RailCamera* railCamera_ = nullptr;

	// 弾
	// std::list<EnemyBullet*> bullets_;

	// 複数の敵に対して
	std::list<EnemyBullet*> enemyBullets_;

	// 複数の敵キャラを管理するリスト
	std::list<Enemy*> enemies_;

	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	// 待機中フラグ
	bool isWaiting = false;
	// 待機タイマー
	int waitingTimer_ = 0;

	// 敵を倒した際のカウンター
	int enemyDeadCounter = 0;

	//ゲームの制限時間
	int gameTimer = 600;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
