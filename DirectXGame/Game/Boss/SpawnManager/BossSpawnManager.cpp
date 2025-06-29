#include "BossSpawnManager.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"

#pragma region ボス
#include"../GingerbreadMan/GingerbreadMan.h"
#include"../Donut/Donut.h"
#pragma endregion


BossSpawnManager::BossSpawnManager(FollowCamera* camera, const DaiEngine::WorldTransform* playerWorld)
{
	//サイズ指定
	bosses_.resize(static_cast<size_t>(BossType::Count));
	//ボスの生成
	bosses_[int(BossType::GingerbreadMan)] = std::make_unique<GingerbreadMan>("GentlmanGuard", camera, playerWorld);
	bosses_[int(BossType::Donut)] =			 std::make_unique<Donut>("DonutAttack1.5", camera, playerWorld);


	std::unique_ptr<GVariGroup> group = std::make_unique<GVariGroup>("BossManager");

	group->SetMonitorValue("nextBoss", &isNextBoss_);

	for (auto& boss : bosses_) {
				//ボスのツリーをセット
		group->SetTreeData(boss->tree_);
	}
}

void BossSpawnManager::Initialize()
{
	//ボスの初期化
	for (auto& boss : bosses_) {
		boss->InitParameters();
	}

	typeRequest_ = (int)BossType::GingerbreadMan;
}

void BossSpawnManager::Update()
{
	//現在のボスの死亡チェック
	CheckBossDead();

	if (typeRequest_) {
		//ボスの生成タイプがリクエストされている場合、ボスを生成
		if (typeRequest_.value() < (int)BossType::Count)
		{
			//リクエストの値にする
			bossType_ = (BossType)typeRequest_.value();
			//リクエストをクリア
			typeRequest_ = std::nullopt; 

		}
		else {
			//全員死んだのでフラグON
			allBossDead_ = true;
		}
	}

	//更新
	bosses_[(int)bossType_]->Update();
}

void BossSpawnManager::Draw()
{
	//ボスの描画
	bosses_[(int)bossType_]->Draw();
}

void BossSpawnManager::UIDraw()
{
	//ボスの描画
	bosses_[(int)bossType_]->DrawUI();
}

void BossSpawnManager::CheckBossDead()
{
	//現在のボスが死んだ場合
	if(bosses_[(int)bossType_]->GetIsDead()||isNextBoss_){

		isNextBoss_=false;

		//次のボスをリクエスト
		typeRequest_ = (int)bossType_ + 1; 

	}
}
