#pragma once
///---------------------------------------------------------------------------------------------
// 
// タイトルシーン
// 
///---------------------------------------------------------------------------------------------


#include "IScene.h"
#include <memory>
#include <list>
#include <map>
#include <string>
#include <functional>

#include "Sprite.h"
#include "Object3d.h"
#include "Particle.h"
#include "Camera.h"
#include "WorldTransform.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Vec2.h"
#include "GPUParticle.h"
#include "Audio.h"


//タイトルシーンクラス
class TitleScene : public DaiEngine::IScene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init()override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 背景描画
	/// </summary>
	void DrawBackGround()override;
	/// <summary>
	/// モデル描画
	/// </summary>
	void DrawModel()override;
	/// <summary>
	/// パーティクル描画
	/// </summary>
	void DrawParticle()override;
	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI()override;
	/// <summary>
	/// ポストエフェクトを掛けるモデルなどの描画
	/// </summary>
	void DrawPostEffect()override;
	/// <summary>
	/// レンダーテクスチャの描画
	/// </summary>
	void DrawRenderTexture()override;
	/// <summary>
	/// デバッグ用
	/// </summary>
	void DebugGUI()override;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene()override;

private:
	/// <summary>
	/// GlobalVariablesに項目の追加
	/// </summary>
	void SetGlobalVariables();
	/// <summary>
	/// GlobalVariablesから変数に代入
	/// </summary>
	void ApplyGlobalVariables();
	

private:
	//カメラ
	DaiEngine::Camera camera_;
	//ポイントライト
	DaiEngine::PointLight pointLight_;
	//スポットライト
	DaiEngine::SpotLight spotLight_;

	//タイトル用BGM
	DaiEngine::Audio* bgm_ = nullptr;

	DaiEngine::Audio* choiceSE_ = nullptr;
	DaiEngine::Audio* doneSE_ = nullptr;

	//タイトルロゴ
	std::unique_ptr<DaiEngine::Sprite> titleLogo_;
	//ゲームスタートUI
	std::unique_ptr<DaiEngine::Sprite> gameStartUI_;
	//ゲーム終了UI
	std::unique_ptr<DaiEngine::Sprite> gameFinishUI_;
	
	//カーソル消去フラグ
	bool isDeleteCursor_ = false;

private://パラメータまとめたやつら

	enum class Select {
		Start,
		Finish,
	};

	std::vector<Select> order_{
		Select::Start,
		Select::Finish,
	};

	std::map<Select, std::function<void()>> onSelect_ = {
		{ Select::Start, [this]() { ToGame(); } },
		{ Select::Finish, [this]() { ToEnd(); } },
	};

	Select select_ = Select::Start;

	enum class UISwitch {
		On,
		Off,
	};

	UISwitch gStartUISwitch_ = UISwitch::On;
	UISwitch gFinishUISwitch_ = UISwitch::Off;


private:

	void MenuInput();

	void ToGame();
	void ToEnd();

};

