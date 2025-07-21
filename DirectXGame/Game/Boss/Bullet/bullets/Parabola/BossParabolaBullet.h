#pragma once
#include"Boss/Bullet/IBossBullet/IBossBullet.h"
#include "Audio.h"

class BossParabolaBullet : public IBossBullet {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="data"></param>
	/// <param name="camera"></param>
	BossParabolaBullet(const BossBulletData& data, DaiEngine::Camera* camera);
	~BossParabolaBullet() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// ブロック接触時の追加処理
	/// </summary>
	void AddOnCollisionBlock()override;

private:

	float currentCount_ = 0;

	DaiEngine::Audio* whipSE_ = nullptr;

};