#include "GameScene.h"
#include "AxisIndicator.h"
#include "MyMath.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
	delete skydome_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");
	enemyTextureHandle_ = TextureManager::Load("mario_yosshi.jpg");

	model_ = Model::Create();

	viewProjection_.Initialize();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &viewProjection_);

	enemy_ = new Enemy();
	enemy_->Initialize(model_, enemyTextureHandle_, &viewProjection_);
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する( アドレス渡し )
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 天球の3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	//天球
	skydome_ = new skydome;
	skydome_->Initialize(modelSkydome_, &viewProjection_);

}

void GameScene::Update() {

	CheckAllCollision();

	player_->Update();
	if (enemy_ != nullptr) {
		enemy_->Update();
	}

	/// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetView();
		viewProjection_.matProjection = debugCamera_->GetProjection();

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}

#endif // _DEBUG

	//天球
	skydome_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	///
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player_->Draw(viewProjection_);
	if (enemy_ != nullptr) {
		enemy_->Draw(viewProjection_);
	}

	//天球
	skydome_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	const float playerRadius = 1.5f;       // 自キャラの半径1.5とする
	const float playerBulletRadius = 1.5f; // 自弾の半径1.5とする
	const float enemyRadius = 1.5f;        // 敵キャラの半径1.5とする
	const float enemyBulletRadius = 1.5f;  // 敵弾の半径1.5とする

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();
	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		// 座標AとBの距離を求める
		float distance = MyMath::Length(posB - posA);
		// 交差判定
		if (distance <= playerRadius + enemyBulletRadius) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();
	// 自弾と敵キャラ全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPosition();
		// 座標AとBの距離を求める
		float distance = MyMath::Length(posB - posA);
		// 交差判定
		if (distance <= playerBulletRadius + enemyRadius) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		for (EnemyBullet* enemyBullet : enemyBullets) {
			posA = playerBullet->GetWorldPosition();
			posB = enemyBullet->GetWorldPosition();
			// 座標AとBの距離を求める
			float distance = MyMath::Length(posB - posA);
			//交差判定
			if (distance <= playerBulletRadius + enemyBulletRadius) {
				// 自弾の衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
#pragma endregion
}
