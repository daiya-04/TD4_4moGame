#pragma once
#include"Boss/Bullet/IBossBullet/IBossBullet.h"

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
	void AddOnCollisionBlock() {};
};