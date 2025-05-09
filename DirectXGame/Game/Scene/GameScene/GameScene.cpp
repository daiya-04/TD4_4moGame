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

GameScene::GameScene() {
}

GameScene::~GameScene() {
	
}


void GameScene::Init(){
	//カメラ初期化
	camera_.Init();
	//ライト初期化
	pointLight_.Init();
	spotLight_.Init();
	//object3dクラスにライトセット
	DaiEngine::Object3d::SetPointLight(&pointLight_);
	DaiEngine::Object3d::SetSpotLight(&spotLight_);

	///

	GameObject::GetCamera(&camera_);
	obj_ = std::make_unique<GameObject>();


	///

}

void GameScene::Update() {
	DebugGUI();

#ifdef _DEBUG
	
	//デバッグ用シーンの切り替えコマンド
	if (DaiEngine::Input::GetInstance()->PushKey(DIK_LCONTROL) && DaiEngine::Input::GetInstance()->TriggerKey(DIK_1)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Title");
	}
	if (DaiEngine::Input::GetInstance()->PushKey(DIK_LCONTROL) && DaiEngine::Input::GetInstance()->TriggerKey(DIK_3)) {
		DaiEngine::SceneManager::GetInstance()->ChangeScene("Debug");
	}

#endif // _DEBUG

	

	//ライト更新
	pointLight_.Update();
	spotLight_.Update();

	//カメラ更新
	camera_.UpdateViewMatrix();
	camera_.UpdateCameraPos();
	
	obj_->Update();
}

void GameScene::DrawBackGround(){

	

}

void GameScene::DrawModel(){

	obj_->Draw();

}

void GameScene::DrawParticleModel(){



}

void GameScene::DrawParticle(){

}

void GameScene::DrawUI(){
	
}

void GameScene::DrawPostEffect() {

	

}

void GameScene::DrawRenderTexture() {
	
}

void GameScene::DebugGUI(){
#ifdef _DEBUG
  
	


#endif // _DEBUG
}
