#include "BossBulletManager.h"

BossBulletManager::BossBulletManager(Boss* boss)
{
	//オブジェクト生成
	InstancingGameObject::Init("Candy", 100);
	//警告円の生成
	dangerZone_ = std::make_unique<InstancingGameObject>();
	dangerZone_->Init("DangerZone", 100);

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

			//警告円の更新
			DaiEngine::InstancingObjData dangerData;
			dangerData.worldTransform_ = data->GetWarningWorld();
			dangerData.worldTransform_.UpdateMatrix();
			dangerZone_->SetData(dangerData);
		}
	}

	//死んだ弾を削除
	bullets_.remove_if([](auto& data) {
		return data->GetDead();
		});
}

void BossBulletManager::Draw()
{
	//描画
	InstancingGameObject::Draw();

	//警告円の描画
	dangerZone_->Draw();
}

void BossBulletManager::SpawnBullet(const DaiEngine::WorldTransform& pos)
{
	Vector3 position = pos.translation_;
	//指定値高くする
	position.y = bulletStartHeight_;

	//渡すパラメータ設定
	BossBulletData data;
	data.world.Init();
	data.velocity = Vector3{ 0,-1.0f,0 }*fallSpeed_;
	data.radius = pos.scale_.x;

	//座標設定
	data.world.translation_ = position;

	//警告円
	data.warningWorld.Init();
	data.warningWorld = pos;;

	//生成
	std::unique_ptr<BossBullet>bullet = std::make_unique<BossBullet>(data);

	//配列に追加
	bullets_.emplace_back(std::move(bullet));
}
