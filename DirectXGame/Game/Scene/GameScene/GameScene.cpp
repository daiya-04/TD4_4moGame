#include "GameScene.h"

#include "DirectXCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"
#include "ImGuiManager.h"
#include "Audio.h"
#include "Input.h"
#include "SceneManager.h"
#include "Hit.h"
#include <random>
#include <algorithm>
#include "ColliderManager.h"
#include "AudioManager.h"
#include "GlobalVariables.h"


GameScene::GameScene() {
	globalVariableManager_ = globalVariableManager_->GetInstance();
}

GameScene::~GameScene() {

	bgm_->StopSound();
}

void GameScene::SetGlobalVariables() {
	DaiEngine::GlobalVariables* globalVariables = DaiEngine::GlobalVariables::GetInstance();


	//AボタンUIの調整項目追加
	std::string groupName = "Attack_Text";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", uis_["Attack_Text"]->GetPosition());
	//ワープホールの調整項目追加
	groupName = "Avoid_Text";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", uis_["Avoid_Text"]->GetPosition());
	//ゲーム開始演出の調整項目追加
	groupName = "Move_Text";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", uis_["Move_Text"]->GetPosition());

	//ゲーム開始演出の調整項目追加
	groupName = "AttackButton";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", uis_["AttackButton"]->GetPosition());

	//ゲーム開始演出の調整項目追加
	groupName = "AvoidButton";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", uis_["AvoidButton"]->GetPosition());

	//ゲーム開始演出の調整項目追加
	groupName = "MoveButton";
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Translation", uis_["MoveButton"]->GetPosition());


}

void GameScene::ApplyGlobalVariables() {
	DaiEngine::GlobalVariables* globalVariables = DaiEngine::GlobalVariables::GetInstance();

	//AボタンUIのパラメータ設定
	std::string groupName = "Attack_Text";
	uis_["Attack_Text"]->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	//ワープホールのパラメータ設定
	groupName = "Avoid_Text";
	uis_["Avoid_Text"]->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	//ゲーム開始演出のパラメータ設定
	groupName = "Move_Text";
	uis_["Move_Text"]->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	//ゲーム開始演出のパラメータ設定
	groupName = "AttackButton";
	uis_["AttackButton"]->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	//ゲーム開始演出のパラメータ設定
	groupName = "AvoidButton";
	uis_["AvoidButton"]->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

	//ゲーム開始演出のパラメータ設定
	groupName = "MoveButton";
	uis_["MoveButton"]->SetPosition(globalVariables->GetVec2Value(groupName, "Translation"));

}

void GameScene::Init() {
	//カメラ初期化
	camera_.Init();
	//ライト初期化
	pointLight_.Init();
	spotLight_.Init();
	//object3dクラスにライトセット
	DaiEngine::Object3d::SetPointLight(&pointLight_);
	DaiEngine::Object3d::SetSpotLight(&spotLight_);

	///

	///
	uint32_t attackTextTex = DaiEngine::TextureManager::Load("attack.png");
	uint32_t avoidTextTex = DaiEngine::TextureManager::Load("avoid.png");
	uint32_t moveTextTex = DaiEngine::TextureManager::Load("move.png");
	uint32_t attackButtonTex = DaiEngine::TextureManager::Load("attackBottom.png");
	uint32_t avoidButtonTex = DaiEngine::TextureManager::Load("avoidBottom.png");
	uint32_t moveButtonTex = DaiEngine::TextureManager::Load("moveBottom.png");

	///

	bgm_ = DaiEngine::AudioManager::Load("BGM/Game.mp3");
	bgm_->Play();

	//ゲームオブジェクトにカメラ設定
	GameObject::SetCamera(&camera_);
	//陰士単シングオブジェクトにカメラ設定
	InstancingGameObject::SetCamera(&camera_);
	

	//プレイヤー生成
	player_ = std::make_unique<Player>();
	playerAttackEffect_ = std::make_unique<PlayerAttackEffect>();
	playerAttackEffect_->Init();
	player_->SetAttackEffect(playerAttackEffect_.get());

	//追従カメラ処理生成
	followCamera_ = std::make_unique<FollowCamera>(&camera_, player_->GetWorld().translation_);
	
	//ボス生成
	//boss_ = std::make_unique<Boss2>(followCamera_.get());
	//boss_->SetPlayerWorld(&player_->GetWorld());

	bossSpawnManager_ = std::make_unique<BossSpawnManager>(followCamera_.get(), &player_->GetWorld());
	player_->SetBossWorld(&bossSpawnManager_->GetBossWorld());


	//地面生成
	field_ = std::make_unique<Field>();
	field_->Initialize();
	///

	///UI

	uis_["Attack_Text"].reset(DaiEngine::Sprite::Create(attackTextTex, {}));
	uis_["Avoid_Text"].reset(DaiEngine::Sprite::Create(avoidTextTex, {}));
	uis_["Move_Text"].reset(DaiEngine::Sprite::Create(moveTextTex, {}));
	uis_["AttackButton"].reset(DaiEngine::Sprite::Create(attackButtonTex, {}));
	uis_["AvoidButton"].reset(DaiEngine::Sprite::Create(avoidButtonTex, {}));
	uis_["MoveButton"].reset(DaiEngine::Sprite::Create(moveButtonTex, {}));

	for (auto& [tag, ui] : uis_) {
		ui->SetScale(0.7f);
	}

	///

	SetGlobalVariables();
	ApplyGlobalVariables();

	//全ての初期化の後に処理
	globalVariableManager_->LoadAllSaveData();
	globalVariableManager_->SetLoadAllData();

	//セットされたデータで初期化
	player_->Init();
	//boss_->Initialize();
	bossSpawnManager_->Initialize();

	field_->CreateStage();
	field_->StartStage();
}

void GameScene::Update() {
	DebugGUI();

	auto* input = DaiEngine::Input::GetInstance();

#ifdef _DEBUG

	///デバッグ用シーン切り替えコマンド
	//「Ctrl + 1」でタイトルシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_1)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Title");
	}
	//「Ctrl + 3」でクリアシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_3)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Clear");
	}
	//「Ctrl + 4」でゲームオーバーシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_4)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("GameOver");
	}
	//「Ctrl + 0」でテストシーンへ
	if (input->PushKey(DIK_LCONTROL) && input->TriggerKey(DIK_0)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Debug");
	}

	ApplyGlobalVariables();

#endif // _DEBUG

	//ライト更新
	pointLight_.Update();
	spotLight_.Update();

	//追従カメラ更新
	followCamera_->Update();

	//カメラ更新
	camera_.UpdateViewMatrix();
	camera_.UpdateCameraPos();

	//ステージ初期化済でプレイヤー更新
	if (!field_->GetStageAnimationFinishedFlag()) {
		//プレイヤー更新
		player_->Update();
		player_->UpdateOnField(field_->GetMassLocationPosY(player_->GetWorld().translation_) + player_->GetWorld().scale_.y);
		player_->SetField(field_.get());

		//ボス更新
		bossSpawnManager_->Update();
		//ボスのワールド座標取得
		player_->SetBossWorld(&bossSpawnManager_->GetBossWorld());
    
    playerAttackEffect_->Update();

	}

	//地面更新
	field_->Update();

	//弾の更新
	for (std::unique_ptr<BossBullet>& bullet : bossSpawnManager_->GetBullets()) {
		Vector2 targetBlock = field_->GetNearestBlockAt(bullet->GetWorld().translation_.x, bullet->GetWorld().translation_.z);
		Block* block = field_->GetBlock(bullet->GetWorld().translation_.x, bullet->GetWorld().translation_.z);

		// Y範囲にあるか判定
		if (block->world.translation_.y >= bullet->GetWorld().translation_.y && block->world.translation_.y <= bullet->GetWorld().translation_.y + bullet->GetWorld().scale_.y) {
			
			//下げる値取得
			float deltaY = field_->GetDeltaY();
			
			//もし上げる弾なら向きを変更
			if (bullet->GetType() != BulletType::Fall) {
				//落下弾以外は上方向に
				deltaY *= -1.0f; 
			}

			//フィールドに影響
			field_->RaiseBlocksAroundWithAttenuation(field_->GetNearestBlockAt(bullet->GetWorld().translation_.x, bullet->GetWorld().translation_.z), bullet->GetWorld().scale_.x * 1.5f, deltaY);
			bullet->OnCollision();
		}
	}

	DaiEngine::InstancingObjData data;
	data.worldTransform_.Init();
	data.worldTransform_.translation_.x = 10;

	//当たり判定処理
	DaiEngine::ColliderManager::GetInstance()->CheckAllCollision();


	//死亡時ゲームおーばーへ
	if (player_->GetIsDead()) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("GameOver");
	}

	//ボスのHPが0以下になったらクリアへ
	if (bossSpawnManager_->GetAllBossDead()) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Clear");
	}
}

void GameScene::DrawBackGround() {



}

void GameScene::DrawModel() {

	//地面描画
	field_->Draw();

	//ボス描画
	bossSpawnManager_->Draw();

	//プレイヤー描画
	player_->Draw();

}

void GameScene::DrawParticle() {

}

void GameScene::DrawUI() {
	for (auto& [tag, ui] : uis_) {
		ui->Draw();
	}
	player_->DrawUI();
	bossSpawnManager_->UIDraw();
}

void GameScene::DrawPostEffect() {



}

void GameScene::DrawRenderTexture() {

}

void GameScene::DebugGUI() {
#ifdef _DEBUG

	//デバッグマネージャの更新
	globalVariableManager_->Update();


#endif // _DEBUG
}
