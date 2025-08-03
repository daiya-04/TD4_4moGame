#pragma once
#include"Boss/Bullet/IBossBullet/IBossBullet.h"
#include "Audio.h"

class BossNoneBullet : public IBossBullet {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BossNoneBullet(const BossBulletData& data, DaiEngine::Camera* camera);
	~BossNoneBullet() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// ブロック接触時の追加処理
	/// </summary>
	void AddOnCollisionBlock();

private:

	DaiEngine::Audio* se_ = nullptr;

};