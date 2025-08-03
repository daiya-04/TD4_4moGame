#include "BossSpawnManager.h"
#include"GlobalVariable/Group/GlobalVariableGroup.h"

#pragma region ボス
#include"../GingerbreadMan/GingerbreadMan.h"
#include"../Donut/Donut.h"
#include"../CupCake/CupCake.h"
#pragma endregion


BossSpawnManager::BossSpawnManager(FollowCamera* camera, const DaiEngine::WorldTransform* playerWorld)
{
	camera_ = camera;

	//サイズ指定
	bosses_.resize(static_cast<size_t>(BossType::Count));
	//ボスの生成
	bosses_[int(BossType::GingerbreadMan)] = std::make_unique<GingerbreadMan>("GentlmanStandby", camera, playerWorld);
	bosses_[int(BossType::Donut)] =			 std::make_unique<Donut>("Donut", camera, playerWorld);
	bosses_[int(BossType::CupCake)] =		 std::make_unique<CupCake>("CapCakeStandby", camera, playerWorld);

	ui_ = std::make_unique<BossUI>();
	ui_->SetBossData(bosses_);

	std::unique_ptr<GVariGroup> group = std::make_unique<GVariGroup>("BossManager");
	group->SetMonitorValue("nowBoss", &nowBossName_);
	group->SetMonitorValue("nextBoss", &isNextBoss_);

	for (auto& boss : bosses_) {
				//ボスのツリーをセット
		group->SetTreeData(boss->tree_);
	}

	bossManager_ = BossManager::GetInstance();
}

void BossSpawnManager::Initialize(BossType bosstype)
{
	//ボスの初期化
	for (auto& boss : bosses_) {
		boss->InitParameters();
	}

	typeRequest_ = (int)bosstype;
}

void BossSpawnManager::Update()
{

#ifdef _DEBUG
	//現在のボス名を取得
	nowBossName_ = bossNames_[(int)bossType_];
#endif // _DEBUG

	//現在のボスの死亡チェック
	CheckBossDead();

	if (typeRequest_) {
		//ボスの生成タイプがリクエストされている場合、ボスを生成
		if (typeRequest_.value() < (int)BossType::Count)
		{
			//リクエストの値にする
			bossType_ = (BossType)typeRequest_.value();

			if (typeRequest_.value() == static_cast<int>(BossType::CupCake)) {
				//UI演出
				ui_->StartFadeOut();

			}

			//リクエストをクリア
			typeRequest_ = std::nullopt; 
			changeBoss_ = false;
			bossManager_->SetBossType(bossType_);

			bossJustDied_ = false;

		}
		else {
			//全員死んだのでフラグON
			allBossDead_ = true;
			bossManager_->SetBossType(BossType::GingerbreadMan);
		}
	}

	if (!changeBoss_) {
		//更新
		bosses_[(int)bossType_]->Update();
	}
	else {
		//次のボスが出るまでの待機時間
		if (currentChangeCount_++ >= changeCount_) {
			//次のボスをリクエスト
			typeRequest_ = (int)bossType_ + 1;
		}
	}

	ui_->Update();
}

void BossSpawnManager::Draw()
{
	if (!changeBoss_) {
		//ボスの描画
		bosses_[(int)bossType_]->Draw();
	}
}

void BossSpawnManager::UIDraw()
{
	//ボスの描画
	bosses_[(int)bossType_]->DrawUI();
	
	ui_->Draw();
}

void BossSpawnManager::SetOnField(float y)
{
	//現在のボス取得
	IBoss* boss = bosses_[(int)bossType_].get();
	//現在の位置
	if (!boss->parameters_.isFly_) {
		boss->SetPositionY(y);
	}
	//フィールドのY取得
	boss->parameters_.fieldY_ = y;
}

void BossSpawnManager::CheckBossDead()
{
	//現在のボスが死んだ場合
	if(!changeBoss_&&(bosses_[(int)bossType_]->GetIsDead()||isNextBoss_)&&bosses_[(int)bossType_]->parameters_.currentBulletNum_==0) {
		
		bosses_[(int)bossType_]->SetAttackColliderOff();
		
		//デバッグ用次ボス生成をOFF
		isNextBoss_=false;

		//引継ぎ時間フラグON
		changeBoss_ = true;
		//カウントリセット
		currentChangeCount_ = 0;
		camera_->SetState(FollowCamera::State::Follow);

		bossJustDied_ = true;
	}
}
