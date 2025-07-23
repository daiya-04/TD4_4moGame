#pragma once
#include "IScene.h"
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <functional>

#include "Sprite.h"
#include "Object3d.h"
#include "Camera.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Audio.h"

#include "Boss/BossManager/BossManager.h"


class GameOverScene : public DaiEngine::IScene {
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
	~GameOverScene()override;


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

	//ゲームオーバー用BGM
	DaiEngine::Audio* bgm_ = nullptr;

	DaiEngine::Audio* choiceSE_ = nullptr;
	DaiEngine::Audio* doneSE_ = nullptr;

	//エームオーバー文字
	std::unique_ptr<DaiEngine::Sprite> gameOverText_;
	//タイトルに戻る
	std::unique_ptr<DaiEngine::Sprite> titleBackUI_;
	//最初から始める
	std::unique_ptr<DaiEngine::Sprite> reStartUI_;
	//途中から
	std::unique_ptr<DaiEngine::Sprite> continueUI_;

private:

	enum class Select {
		TitleBack,
		Continue,
		ReStart,
	};

	std::vector<Select> order_{
		Select::ReStart,
		Select::Continue,
		Select::TitleBack,
	};

	std::map<Select, std::function<void()>> onSelect_ = {
		{ Select::TitleBack, [this]() { ToTitleBack(); } },
		{ Select::Continue, [this]() { ToContinue(); } },
		{ Select::ReStart, [this]() { ToReStart(); } },
	};

	Select select_ = Select::TitleBack;

	enum class UISwitch {
		On,
		Off,
	};

	UISwitch gTitleBackUISwitch_ = UISwitch::Off;
	UISwitch gReStartUISwitch_ = UISwitch::Off;
	UISwitch gContinueUISwitch_ = UISwitch::On;

	BossManager* bossManager_ = nullptr;

private:

	void MenuInput();

	void ToTitleBack();
	void ToContinue();
	void ToReStart();

};

