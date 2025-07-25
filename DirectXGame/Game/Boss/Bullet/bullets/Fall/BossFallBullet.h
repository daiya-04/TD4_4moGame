#pragma once
#include"Boss/Bullet/IBossBullet/IBossBullet.h"
#include "Audio.h"

class BossFallBullet : public IBossBullet {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="data"></param>
	/// <param name="camera"></param>
	BossFallBullet(const BossBulletData& data, DaiEngine::Camera* camera);
	~BossFallBullet()=default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// ブロック接触時の追加処理
	/// </summary>
	void AddOnCollisionBlock()override;

private:

	DaiEngine::Audio* candySE_ = nullptr;

};