#include "GameScene.h"
#include "AxisIndicator.h"
#include "MyMath.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;
	delete player_;
	delete debugCamera_;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("sample.png");
	enemyTextureHandle_ = TextureManager::Load("mario_yosshi.jpg");
	//レティクルのテクスチャ
	TextureManager::Load("point.png");

	model_ = Model::Create();

	viewProjection_.Initialize();
	worldTransform_.Initialize();

	// RailCamera の初期化
	railCamera_ = new RailCamera();
	railCamera_->Initialize(worldTransform_, &viewProjection_);

	player_ = new Player();
	Vector3 playerPosition(0, 0, 20);
	player_->Initialize(model_, textureHandle_, playerPosition);
	player_->SetParent(railCamera_->GetWorldTransform());

	// enemy_ = new Enemy();
	// enemy_->Initialize(model_, enemyTextureHandle_, &viewProjection_);
	//// 敵キャラにゲームシーンを渡す
	// enemy_->SetGameScene(this);
	//// 敵キャラに自キャラのアドレスを渡す
	// enemy_->SetPlayer(player_);
	////
	// enemies_.push_back(enemy_);

	// デスフラグの立った弾を削除( 敵 )
	// if (enemy_->isDead()) {
	//}

	//// デスフラグの立った弾を削除( 敵弾 )
	// enemyBullets_.remove_if([](EnemyBullet* bullet) {
	//	if (bullet->isDead()) {
	//		delete bullet;
	//		return true;
	//	}
	//	return false;
	// });

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する( アドレス渡し )
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 天球の3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	// 天球
	skydome_ = new skydome;
	skydome_->Initialize(modelSkydome_, &viewProjection_);
}

void GameScene::Update() {

	LoadEnemyPopData();
	CheckAllCollision();
	UpdateEnemyPopCommands();

	player_->Update(viewProjection_);
	/*if (enemy_ != nullptr) {
	    enemy_->Update();
	}*/

	/// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetView();
		viewProjection_.matProjection = debugCamera_->GetProjection();

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// RailCamera の更新
		railCamera_->Update(); // RailCamera の更新を行う
		// ビュープロジェクション行列の更新と転送
		viewProjection_.matProjection =
		    railCamera_->GetViewProjection().matProjection; // プロジェクション行列を取得してコピー
		viewProjection_.TransferMatrix(); // 必要に応じてビュー行列を更新する
	}

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}

#endif // _DEBUG

	// 天球
	skydome_->Update();

	// enemyBullets
	for (EnemyBullet* enemyBullet : enemyBullets_) {
		enemyBullet->Update();
	}

	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->isDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	// 弾の更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->isDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// ワールドトランスフォーム
	worldTransform_.UpdateMatrix();
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

	for (Enemy* enemy : enemies_) {
		if (!enemy->isDead()) {
			if (enemy != nullptr) {
				enemy->Draw(viewProjection_);
			}
		}
	}

	// 天球
	skydome_->Draw();

	// 弾の描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	player_->DrawUI();

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
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;

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
	// 敵キャラのリストが存在する場合
	for (Enemy* enemy : enemies_) {
		// 敵キャラの座標
		posA = enemy->GetWorldPosition();
		// 全ての自弾と敵キャラ全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			posB = bullet->GetWorldPosition();
			// 座標AとBの距離を求める
			float distance = MyMath::Length(posB - posA);
			// 交差判定
			if (distance <= playerBulletRadius + enemyRadius) {
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
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
			// 交差判定
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::LoadEnemyPopData() {
	// ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	// 待機処理
	if (isWaiting) {
		waitingTimer_--;
		if (waitingTimer_ <= 0) {
			// 待機完了
			isWaiting = false;
		}
		return;
	}

	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		// 1行分のもし列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を飛ばす
			continue;
		}

		// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			EnemyPop(Vector3(x, y, z));
		} else if (word.find("WAIT") == 0) { // WAITコマンド
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			////待機開始
			// 待機中フラグ
			isWaiting = true;
			// 待機タイマー
			waitingTimer_ = waitTime;

			// コマンドループを抜ける
			break;
		}
	}
}

void GameScene::EnemyPop(Vector3 position) {
	enemy_ = new Enemy();
	enemy_->Initialize(model_, enemyTextureHandle_, &viewProjection_, position);
	// 敵キャラにゲームシーンを渡す
	enemy_->SetGameScene(this);
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	//
	// enemy_->SetPos(&position);
	//
	enemies_.push_back(enemy_);
}
