#include "DangerZoneManager.h"
#include"Boss/Boss.h"

DangerZoneManager::DangerZoneManager(Boss* boss)
{
	//オブジェクト生成
	InstancingGameObject::Init("DangerZone", 100);

	//ボスポインタ取得
	boss_ = boss;

	tree_.name_ = "DangerZone";
	tree_.SetValue("warningSphereHeight", &warningHeight_);
	tree_.SetValue("warningCount", &warningTime_);
	tree_.SetValue("radius", &radius_);
}

void DangerZoneManager::Update()
{
	//更新して座標設定
	for (auto& data : dangerZones_) {
		data->Update();
		//死んでいない場合データをセット
		if (!data->GetDead()) {
			//専用変数生成
			DaiEngine::InstancingObjData objData;
			//ワールドデータコピー
			objData.worldTransform_ = data->GetWorld();
			//行列更新
			objData.worldTransform_.UpdateMatrix();
			objData.color_ = Vector4(1, 0, 0, 1);
			//データセット
			SetData(objData);
		}
	}

	//死んだ円を削除
	dangerZones_.remove_if([&](auto& data) {
		//死んでいる場合
		if (data->GetDead()) {
			//弾を出現させて削除
			boss_->SpawnBullet(data->GetWorld());

			return true;
		}
		return false;
		});

}

void DangerZoneManager::SpawnDangerZone(const Vector3& position)
{
	Vector3 pos = position;
	////高さ設定
	pos.y += warningHeight_;

	DangerZoneParameters param;
	param.world.Init();
	param.world.translation_ = pos;
	param.world.rotation_.y = (float)std::numbers::pi; //Y軸回転
	param.maxDeadSec = warningTime_;
	param.maxRadius = radius_;

	//新しく生成
	std::unique_ptr<DangerZone>dangerZone = std::make_unique<DangerZone>(param);

	//配列に追加
	dangerZones_.emplace_back(std::move(dangerZone));
}
