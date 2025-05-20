#include "BossBulletManager.h"

BossBulletManager::BossBulletManager(Boss* boss)
{
	//オブジェクト生成
	InstancingGameObject::Init("Standing", 100);
	//ボスのポインタ取得
	boss_ = boss;

	tree_.name_ = "FallingBullet";
	tree_.SetValue("spawnHeight", &bulletStartHeight_);
	tree_.SetValue("fallSpeed", &fallSpeed_);
	tree_.SetValue("radius", &radius_);
}

void BossBulletManager::Update()
{
	//更新して座標設定
	for (auto& data : bullets_) {
		data->Update();
		//死んでいない場合データをセット
		if (!data->GetDead()) {
			//専用変数生成
			DaiEngine::InstancingObjData objData;
			//ワールドデータコピー
			objData.worldTransform_ = data->GetWorld();
			//行列更新
			objData.worldTransform_.UpdateMatrix();
			//データセット
			SetData(objData);
		}
	}

	//死んだ弾を削除
	bullets_.remove_if([](auto& data) {
		return data->GetDead();
		});
}

void BossBulletManager::SpawnBullet(const Vector3& pos)
{
	Vector3 position = pos;
	//指定値高くする
	position.y = bulletStartHeight_;

	//渡すパラメータ設定
	BossBulletData data;
	data.world.Init();
	data.velocity = Vector3{ 0,-1.0f,0 }*fallSpeed_;
	data.radius = radius_;

	//座標設定
	data.world.translation_ = position;
	//生成
	std::unique_ptr<BossBullet>bullet = std::make_unique<BossBullet>(data);

	//配列に追加
	bullets_.emplace_back(std::move(bullet));
}
