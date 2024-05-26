#include "GameScene.h"
#include "TextureManager.h"
#include <WorldTransformEX.cpp>
#include <cassert>
// #include "MyMath.h"

#include <cassert>
#include <cmath>

// class WorldTransform;
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;

	// 単位行列に設定します
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = (i == j) ? 1.0f : 0.0f;
		}
	}

	// 拡大成分を設定します
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;

	return result;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
	           matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
	           matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
	           matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
	          1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	float c = std::cos(radian);
	float s = std::sin(radian);

	Matrix4x4 rotationMatrix;

	rotationMatrix.m[0][0] = 1.0f;
	rotationMatrix.m[0][1] = 0.0f;
	rotationMatrix.m[0][2] = 0.0f;
	rotationMatrix.m[0][3] = 0.0f;
	rotationMatrix.m[1][0] = 0.0f;
	rotationMatrix.m[1][1] = c;
	rotationMatrix.m[1][2] = s;
	rotationMatrix.m[1][3] = 0.0f;
	rotationMatrix.m[2][0] = 0.0f;
	rotationMatrix.m[2][1] = -s;
	rotationMatrix.m[2][2] = c;
	rotationMatrix.m[2][3] = 0.0f;
	rotationMatrix.m[3][0] = 0.0f;
	rotationMatrix.m[3][1] = 0.0f;
	rotationMatrix.m[3][2] = 0.0f;
	rotationMatrix.m[3][3] = 1.0f;

	return rotationMatrix;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	float c = std::cos(radian);
	float s = std::sin(radian);

	Matrix4x4 rotationMatrix;

	rotationMatrix.m[0][0] = c;
	rotationMatrix.m[0][1] = 0.0f;
	rotationMatrix.m[0][2] = -s;
	rotationMatrix.m[0][3] = 0.0f;
	rotationMatrix.m[1][0] = 0.0f;
	rotationMatrix.m[1][1] = 1.0f;
	rotationMatrix.m[1][2] = 0.0f;
	rotationMatrix.m[1][3] = 0.0f;
	rotationMatrix.m[2][0] = s;
	rotationMatrix.m[2][1] = 0.0f;
	rotationMatrix.m[2][2] = c;
	rotationMatrix.m[2][3] = 0.0f;
	rotationMatrix.m[3][0] = 0.0f;
	rotationMatrix.m[3][1] = 0.0f;
	rotationMatrix.m[3][2] = 0.0f;
	rotationMatrix.m[3][3] = 1.0f;

	return rotationMatrix;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	float c = std::cos(radian);
	float s = std::sin(radian);

	Matrix4x4 rotationMatrix;

	rotationMatrix.m[0][0] = c;
	rotationMatrix.m[0][1] = s;
	rotationMatrix.m[0][2] = 0.0f;
	rotationMatrix.m[0][3] = 0.0f;
	rotationMatrix.m[1][0] = -s;
	rotationMatrix.m[1][1] = c;
	rotationMatrix.m[1][2] = 0.0f;
	rotationMatrix.m[1][3] = 0.0f;
	rotationMatrix.m[2][0] = 0.0f;
	rotationMatrix.m[2][1] = 0.0f;
	rotationMatrix.m[2][2] = 1.0f;
	rotationMatrix.m[2][3] = 0.0f;
	rotationMatrix.m[3][0] = 0.0f;
	rotationMatrix.m[3][1] = 0.0f;
	rotationMatrix.m[3][2] = 0.0f;
	rotationMatrix.m[3][3] = 1.0f;

	return rotationMatrix;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result;

	// 単位行列に設定します
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i == j) {
				result.m[i][j] = 1.0f;
			} else {
				result.m[i][j] = 0.0f;
			}
		}
	}

	// 移動成分を設定します
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	return result;
}

Matrix4x4 MakeAffineMatrix(Vector3 scale, Vector3 rotate, Vector3 translate) {
	Matrix4x4 affineMatrix;

	// 各変換行列を作成
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	// 各変換行列を合成してアフィン変換行列を作成
	affineMatrix = Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));

	return affineMatrix;
}

void WorldTransform::UpdateMatrix() {

	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	TransferMatrix();
}

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete debugCamera_;
	delete modelSkydome_;
	delete skydome_;
	delete mapChipField_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	model_ = Model::Create();

	viewProjection_.Initialize();

	modelSkydome_ = Model::CreateFromOBJ("sphere", true);

	skydome_ = new Skydome;
	skydome_->Initiaize(modelSkydome_, &viewProjection_);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// worldTransform_.Initialize();

	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;
	worldTransformBlocks_.resize(kNumBlockVirtical);

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			if ((i + j) % 2 == 0) {
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
			}
		}
	}

	debugCamera_ = new DebugCamera(1280, 720);

	GenerateBlocks();

}

void GameScene::Update() {

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock) {
				continue;
			}

			worldTransformBlock->matWorld_ = MakeAffineMatrix(
			    worldTransformBlock->scale_, worldTransformBlock->rotation_,
			    worldTransformBlock->translation_);

			worldTransformBlock->TransferMatrix();
		}
	}

	debugCamera_->Update();

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}

#endif // _DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetView();
		viewProjection_.matProjection = debugCamera_->GetProjection();

		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}

	skydome_->Update();

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

	/*for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {

			if (!worldTransformBlock) {
				continue;
			}

			model_->Draw(*worldTransformBlock, viewProjection_);
		}
	}*/

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

void GameScene::GenerateBlocks() {

	//要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
	
	worldTransformBlocks_.resize(numBlockVirtical);

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {

			if (mapChipField_->GetMapChipTypeByIndex(j,i)==MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j]=worldTransform;
				worldTransformBlocks_[i][j]->translation_ =
				    mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}

}
