#include "BaseChar.h"



void BaseChar::Init(uint32_t textureHandle, size_t charNum) {

	ch_.reset(DaiEngine::Sprite::Create(textureHandle, {}));
	charNum_ = charNum;

}

void BaseChar::Update() {

	ch_->SetPosition(pos_);
}

void BaseChar::Draw() {

	ch_->Draw();

}
