#include "DangerZoneManager.h"

DangerZoneManager::DangerZoneManager(DangerZoneType type)
{
	//オブジェクト生成
	InstancingGameObject::Init("DangerZone", 100);

	//生成タイプ指定
	type_ = type;


	//デバッグ用にツリーを生成
	tree_.name_ = "DangerZone";
	tree_.SetValue("warningSphereHeight", &warningHeight_);
	tree_.SetValue("warningCount", &warningTime_);
	tree_.SetValue("finalWarningCount", &finalWarningTime_);
	tree_.SetValue("BlinkingNum",&blinkingNum_);
	tree_.SetValue("radius", &radius_);
	tree_.SetValue("alpha", &color_);
	if(type_ == DangerZoneType::Follow) {
		tree_.SetValue("spawnNum", &spawnNum_);
		tree_.SetValue("followSpd", &followSpeed_);
		tree_.SetValue("distance", &distance_);
	}
}

void DangerZoneManager::Update()
{
	//更新して座標設定
	for (auto& data : dangerZones_) {
		data->Update();
		//死んでいない&描画ON場合データをセット
		if (!data->GetIsDead()&&data->GetIsDraw()) {
			//専用変数生成
			DaiEngine::InstancingObjData objData;
			//ワールドデータコピー
			objData.worldTransform_ = data->GetWorld();
			//行列更新
			objData.worldTransform_.UpdateMatrix();
			objData.color_ = color_;
			//データセット
			SetData(objData);
		}
	}

	////死んだ円を削除
	//dangerZones_.remove_if([&](auto& data) {
	//	//死んでいる場合
	//	if (data->GetIsDead()) {
	//		//弾を出現させて削除
	//		param_->SpawnBullet(data->GetWorld());

	//		return true;
	//	}
	//	return false;
	//	});

}

// ラジアンでベクトルを回転させる関数
Vector2 RotateVectorRad(const Vector2& vec, float radians) {
	float cosTheta = std::cos(radians);
	float sinTheta = std::sin(radians);

	return {
		vec.x * cosTheta - vec.y * sinTheta,
		vec.x * sinTheta + vec.y * cosTheta
	};
}

void DangerZoneManager::SpawnDangerZone(const Vector3& position)
{
	if (type_ == DangerZoneType::Normal) {
		Vector3 pos = position;
		////高さ設定
		pos.y = warningHeight_;

		DangerZoneParameters param;
		param.type = type_;
		param.world.Init();
		param.world.translation_ = pos;
		param.world.rotation_.y = (float)std::numbers::pi; //Y軸回転	
		param.maxRadius = radius_;
		param.maxWarningCount = warningTime_;
		param.maxFinalWarningCount = finalWarningTime_;
		param.blinkingNum = blinkingNum_;

		//新しく生成
		std::unique_ptr<DangerZone>dangerZone = std::make_unique<DangerZone>(param);

		//配列に追加
		dangerZones_.emplace_back(std::move(dangerZone));
	}
	else if (type_ == DangerZoneType::Follow) {
		//float rad = (float)std::numbers::pi * 2.0f;
		//プレイヤー四隅に設置
		for(int num=0;num<spawnNum_;num++){
			//プレイヤーの座標を取得
			Vector3 pos = position;

			////プレイヤーの座標を中心に半径分ずらす
			Vector2 offset = Vector2(1.0f, 0.0f);
			////角度を計算
			//float angle = (rad / (float)spawnNum_) * num;
			////ベクトルを回転
			//offset = RotateVectorRad(offset, angle)*distance_;

			//仮で設定
			if (num == 0) {
				offset = Vector2{ 1.0f,0.0f } *distance_;
			}
			else if (num == 1) {
				offset = Vector2{ 0.0f,1.0f } *distance_;
			}
			else if (num == 2) {
				offset = Vector2{ -1.0f,0.0f } *distance_;
			}
			else if (num == 3) {
				offset = Vector2{ 0.0f,-1.0f } *distance_;
			}

			//座標設定
			pos.x += offset.x;
			pos.z += offset.y;
			pos.y = warningHeight_;

			//パラメータ設定
			DangerZoneParameters param;
			param.type = type_;
			param.world.Init();
			param.world.translation_ = pos;
			param.world.rotation_.y = (float)std::numbers::pi; //Y軸回転
			//半径設定
			param.maxRadius = radius_;
			//各時間設定
			param.maxWarningCount = warningTime_;
			param.maxFinalWarningCount = finalWarningTime_;
			param.blinkingNum = blinkingNum_;
			param.playerPos = &position;
			param.followSpd = followSpeed_;
			//新しく生成
			std::unique_ptr<DangerZone>dangerZone = std::make_unique<DangerZone>(param);
			dangerZone->SetPlayerPos(&position);
			//配列に追加
			dangerZones_.emplace_back(std::move(dangerZone));
		}
	}
}
