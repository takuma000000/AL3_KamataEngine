#include "Player.h"
#include "cassert"


void Player::Initialize(Model* model, uint32_t textureHandle) {

	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	//worldTransform_ = worldTransform;

}

void Player::Update() {

	worldTransform_.TransferMatrix();

}

void Player::Draw() {}
