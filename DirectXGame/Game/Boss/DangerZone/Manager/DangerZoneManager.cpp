#include "DangerZoneManager.h"
#include<memory>
#include <random>

#include"Boss/DangerZone/Zones/Simple/SinpleDangerZone.h"
#include"Boss/DangerZone/Zones/Follow/FollowDangerZone.h"
#include"Boss/DangerZone/Zones/Quick/QuickDangerZone.h"


DangerZoneManager::DangerZoneManager()
{
	//オブジェクト生成
	InstancingGameObject::Init("DangerZone", 100);

	//デバッグ用にツリーを生成
	tree_.name_ = "DangerZone";
	tree_.SetValue("warningSphereHeight", &warningHeight_);
	tree_.SetValue("warningCount", &warningTime_);
	tree_.SetValue("finalWarningCount", &finalWarningTime_);
	tree_.SetValue("BlinkingNum", &blinkingNum_);
	tree_.SetValue("radius", &radius_);
	tree_.SetValue("alpha", &color_);
	GvariTree tree;
	tree.name_ = "FollowParam";
	tree.SetValue("followSpd", &followSpeed_);
	tree.SetValue("distance", &distance_);
	tree_.SetTreeData(tree);
	GvariTree btree;
	btree.name_ = "baramaki";
	btree.SetValue("spawnNum", &spawnNum_);
	btree.SetValue("spawnRange", &spawnRange_);
	tree_.SetTreeData(btree);
}


void DangerZoneManager::Update()
{
	//更新して座標設定
	for (auto& data : dangerZones_) {
		data->Update();
		//死んでいない&描画ON場合データをセット
		if (!data->GetIsDead() && data->GetIsDraw()) {
			//専用変数生成
			DaiEngine::InstancingObjData objData;
			//ワールドデータコピー
			objData.worldTransform_ = data->GetWorld();
			//行列更新
			objData.worldTransform_.UpdateMatrix();
			objData.color_ = color_;
			objData.alpha_ = color_.w;
			//データセット
			SetData(objData);
		}
	}
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




float RandomFloat(float min, float max) {
	static std::random_device rd;  // 一度だけ初期化
	static std::mt19937 mt(rd());  // メルセンヌ・ツイスタ
	std::uniform_real_distribution<float> dist(min, max);
	return dist(mt);
}

void DangerZoneManager::SpawnDangerZone(const Vector3& Ppos,BulletType type)
{
	//上から落下タイプの弾処理
	if (type == BulletType::Fall) {
		Vector3 pos = Ppos;
		////高さ設定
		pos.y = warningHeight_;

		//値生成
		DangerZoneParameters param;
		param.world.translation_ = pos;
		param.type = type;
		SetParameters(param);

		//新しく生成
		std::unique_ptr<IDangerZone>dangerZone = std::make_unique<SinpleDangerZone>(param);
		dangerZone->SetPlayerPos(&Ppos);
		//配列に追加
		dangerZones_.emplace_back(std::move(dangerZone));

	}//追従する
	else if (type == BulletType::None) {
		//float rad = (float)std::numbers::pi * 2.0f;
		//プレイヤー四隅に設置
		for (int num = 0; num < 4; num++) {
			//プレイヤーの座標を取得
			Vector3 pos = Ppos;

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

			//値生成
			DangerZoneParameters param;
			//パラメータ設定
			param.world.translation_ = pos;
			param.type = type;
			SetParameters(param);
			//新しく生成
			std::unique_ptr<IDangerZone>dangerZone = std::make_unique<FollowDangerZone>(param);
			dangerZone->SetPlayerPos(&Ppos);
			//配列に追加
			dangerZones_.emplace_back(std::move(dangerZone));
		}
	}//放物線処理
	else if (type == BulletType::Parabola) {
		//ランダムな所に生成
		for (int i = 0; i < spawnNum_; i++) {
			//ランダムな値XZ取得
			float length =RandomFloat(spawnRange_.x, spawnRange_.y);

			//ランダム向きベクトル生成
			Vector3 pos = {
				RandomFloat(-1.0f,1.0f),
				0,
				RandomFloat(-1.0f,1.0f)
			};

			//正規化して出た長さにする
			pos = pos.Normalize() * length;
			//高さは指定分
			pos.y = warningHeight_;

			//値生成
			DangerZoneParameters param;
			//パラメータ設定
			param.world.translation_ = pos;
			param.type = type;
			SetParameters(param);
			//新しく生成
			std::unique_ptr<IDangerZone>dangerZone = std::make_unique<SinpleDangerZone>(param);
			dangerZone->SetPlayerPos(&Ppos);
			//配列に追加
			dangerZones_.emplace_back(std::move(dangerZone));
		}
	}
	else if (type == BulletType::Wave) {
		Vector3 pos = {0,0,0};
		//値生成
		DangerZoneParameters param;
		param.world.translation_ = pos;
		param.type = type;
		SetParameters(param);
		//新しく生成
		std::unique_ptr<IDangerZone>dangerZone = std::make_unique<QuickDangerZone>(param);
		//
		dangerZone->SetPlayerPos(&Ppos);
		//配列に追加
		dangerZones_.emplace_back(std::move(dangerZone));
	}
	else if (type == BulletType::Dive) {
		Vector3 pos = { 0,0,0 };
		//値生成
		DangerZoneParameters param;
		param.world.translation_ = pos;
		param.type = type;
		SetParameters(param);
		//新しく生成
		std::unique_ptr<IDangerZone>dangerZone = std::make_unique<QuickDangerZone>(param);
		//
		dangerZone->SetPlayerPos(&Ppos);
		//配列に追加
		dangerZones_.emplace_back(std::move(dangerZone));
	}
}

void DangerZoneManager::SetParameters(DangerZoneParameters& param)
{
	param.world.Init();
	//Y軸回転	
	param.world.rotation_.y = (float)std::numbers::pi;
	param.maxRadius = radius_;
	param.maxWarningCount = warningTime_;
	param.maxFinalWarningCount = finalWarningTime_;
	param.blinkingNum = blinkingNum_;
	param.followSpd = followSpeed_;
}
