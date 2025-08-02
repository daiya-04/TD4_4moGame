#pragma once
#include "Sprite.h"
#include "Vec2.h"
#include <string>
#include <memory>


class BaseChar {
public:

	virtual void Init(uint32_t textureHandle, size_t charNum);

	virtual void Update();

	void Draw();

	void SetPos(const Vector2& pos) { pos_ = pos; }

protected:

	size_t charNum_ = 0;
	std::unique_ptr<DaiEngine::Sprite> ch_;

	Vector2 pos_{};

};

