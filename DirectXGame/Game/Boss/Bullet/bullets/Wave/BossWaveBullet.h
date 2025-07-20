#pragma once
#include"Boss/Bullet/IBossBullet/IBossBullet.h"

class BossWaveBullet : public IBossBullet {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="data"></param>
	/// <param name="camera"></param>
	BossWaveBullet(const BossBulletData& data, DaiEngine::Camera* camera);
	~BossWaveBullet() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// ブロック接触時の追加処理
	/// </summary>
	void AddOnCollisionBlock()override;

};