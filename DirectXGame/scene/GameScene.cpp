#include "GameScene.h"
#include "TextureManager.h"
#include <WorldTransform.h>
#include <cassert>
// #include "MyMath.h"

#include <cassert>
#include <cmath>

// class WorldTransform;

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	delete debugCamera_;
	delete modelSkydome_;
	delete skydome_;
	delete mapChipField_;
	delete player_;
	delete modelPlayer_;
	delete cameraController_;
	delete modelBlock_;
	delete enemy_;
	delete modelEnemy_;
}

void GameScene::GenerateBlocks() {

	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {

			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ =
				    mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::CheckAllCollisions() {
#pragma region 自キャラと敵キャラの当たり判定

	//判定対象1と2の座標
	AABB aabb1, aabb2;
	//自キャラの座標
	aabb1 = player_->GetAABB();
	//自キャラと敵弾全ての当たり判定
	for (Enemy* enemy : enemies_) {
		//敵弾の座標
		aabb2 = enemy->GetAABB();
		//AABB同士の交差判定
		if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		    (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		    (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision(enemy);
			//敵弾の衝突時コールバックを呼び出す
			enemy->OnCollision(player_);
		}
	}

#pragma endregion
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	model_ = Model::Create();

	viewProjection_.Initialize();

	modelSkydome_ = Model::CreateFromOBJ("sphere", true);
	modelPlayer_ = Model::CreateFromOBJ("catCube", true);
	modelBlock_ = Model::CreateFromOBJ("block", true);
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	skydome_ = new Skydome;
	skydome_->Initiaize(modelSkydome_, &viewProjection_);

	//
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	//
	player_ = new Player();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	// カメラコントローラーの初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	// enemy
	for (int32_t i = 0; i < 3; ++i) {

		Enemy* newEnemy = new Enemy();
		//Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(i, 18);
		Vector3 enemyPosition = {i * 7.0f, 2.0f, 0.0f}; // 各敵のx座標を10ずつ増加
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	movaleArea_ = {100.0f, -100.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(movaleArea_);

	debugCamera_ = new DebugCamera(1280, 720);

	GenerateBlocks();
}

void GameScene::Update() {

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock) {
				continue;
			}

			worldTransformBlock->UpdateMatrix();
		}
	}

	// 敵の当たり判定->更新
	for (Enemy* enemy : enemies_) {
		enemy->Updata();
	}

	debugCamera_->Update();

	player_->Update();

	

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {

		isDebugCameraActive_ = true;
	}

#endif // _DEBUG

	cameraController_->Update();

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetView();
		viewProjection_.matProjection = debugCamera_->GetProjection();

		viewProjection_.TransferMatrix();
	} else {

		viewProjection_.matView = cameraController_->GetView();
		viewProjection_.matProjection = cameraController_->GetProjection();

		viewProjection_.TransferMatrix();
	}

	//
	skydome_->Update();

	//全ての当たり判定を行う
	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
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

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock) {
				continue;
			}

			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// 敵の当たり判定->描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	skydome_->Draw();

	player_->Draw();
	

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
