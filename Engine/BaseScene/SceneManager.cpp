#include "SceneManager.h"
#include "SceneFactory.h"
#include "DirectXCommon.h"
#include "Object3d.h"
#include "Particle.h"
#include "Line.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>

namespace DaiEngine {
	SceneManager* SceneManager::GetInstance() {
		static SceneManager instance;

		return &instance;
	}

	void SceneManager::Init() {

		assert(sceneFactory_);

		fadeTex_ = TextureManager::Load("white.png");

		fade_.reset(Sprite::Create(fadeTex_, { 640.0f,360.0f }));
		fade_->SetSize({ 1280.0f,720.0f });
		fade_->SetColor({ 1.0f,1.0f,1.0f,alpha_ });

#ifdef NDEBUG
		scene_ = sceneFactory_->CreateScene("Title");
#endif // NDEBUG

#ifdef _DEBUG
		scene_ = sceneFactory_->CreateScene("Debug");
#endif // _DEBUG

		scene_->Init();

	}

	void SceneManager::Update() {

		// 次のシーンが存在し、かつフェードインが完了していない場合
		if (nextScene_ && alpha_ < 1.0f) {
			alpha_ += 0.03f;
			if (alpha_ >= 1.0f) {
				scene_ = std::move(nextScene_);
				scene_->Init();
			}
		}

		//次のシーンが存在せず、かつフェードアウトが完了していない場合
		if (!nextScene_ && alpha_ > 0.0f) {
			alpha_ -= 0.03f;
		}

		alpha_ = std::clamp(alpha_, 0.0f, 1.0f);

		//次のシーンが存在せず、フェードアウトが完了している場合
		if (!nextScene_ && alpha_ <= 0.0f) {
			scene_->Update();
		}

		fade_->SetColor({ 1.0f,1.0f,1.0f,alpha_ });

#ifdef _DEBUG

		ImGui::Begin("window");

		ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);

		ImGui::End();



#endif // _DEBUG

	}

	void SceneManager::Draw([[maybe_unused]] ID3D12GraphicsCommandList* commandList) {

		DirectXCommon::GetInstance()->ClearDepthBaffer();

		scene_->DrawPostEffect();

		DirectXCommon::GetInstance()->preDraw();

		///背景スプライト
		Sprite::preDraw();

		scene_->DrawBackGround();

		Sprite::postDraw();

		DirectXCommon::GetInstance()->ClearDepthBaffer();

		scene_->DrawRenderTexture();

		///3dオブジェクト

		Object3d::preDraw();

		scene_->DrawModel();

		Object3d::postDraw();

		///パーティクル
		Particle::preDraw();

		scene_->DrawParticle();

		Particle::postDraw();

		Line::preDraw();

		Line::AllDraw();

		Line::postDraw();

		///UI
		Sprite::preDraw();

		scene_->DrawUI();
		fade_->Draw();

		Sprite::postDraw();

		ImGuiManager::GetInstance()->Draw();

		DirectXCommon::GetInstance()->postDraw();


	}

	void SceneManager::ChangeScene(const std::string& sceneName) {

		assert(nextScene_ == nullptr);

		nextScene_ = sceneFactory_->CreateScene(sceneName);
	}


}
