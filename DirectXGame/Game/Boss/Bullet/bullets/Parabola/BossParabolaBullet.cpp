#include "BossParabolaBullet.h"
#include "EffectManager.h"

BossParabolaBullet::BossParabolaBullet(const BossBulletData& data, DaiEngine::Camera* camera)
{
	IBossBullet::Init(data, camera);
}

void BossParabolaBullet::Update()
{
	//放物線を描く弾の処理
	currentCount_++;

	//カウントが経過で判定
	if (!isActive_ && currentCount_ >= ((float)data_.arriveCount / 2.0f))isActive_ = true;

	//進行度T取得
	float t = (float)currentCount_ / (float)data_.arriveCount;
	//X,Zの移動処理
	float x = Lerp(t, data_.bossWorld.translation_.x, data_.warningWorld.translation_.x);
	float z = Lerp(t, data_.bossWorld.translation_.z, data_.warningWorld.translation_.z);

	//Yの移動処理(真ん中で最高高度
	float y = Lerp(t, data_.bossWorld.translation_.y, data_.warningWorld.translation_.y);
	//放物線の高さを加える
	y += data_.parabolaHeight * (1.0f - std::abs(2.0f * t - 1.0f));
	//ワールドの更新
	data_.world.translation_ = { x, y, z };

	if (currentCount_ >= data_.arriveCount) {
		//到着したら削除
		//isDead_ = true;
		//コライダーを無効にする
		//collider_->ColliderOff();
		//return;
	}

	IBossBullet::UpdateObject();
}

void BossParabolaBullet::AddOnCollisionBlock()
{
	EffectManager::GetInstance()->Trigger("CandyFinishEffect", data_.world.GetWorldPos());
}
