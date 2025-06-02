#pragma once
#include "IScene.h"
#include <memory>
#include <vector>
#include <string>

#include "Sprite.h"
#include "Object3d.h"
#include "Camera.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Audio.h"


class ClearScene : public DaiEngine::IScene {
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
	~ClearScene()override;

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

	//クリア用BGM
	DaiEngine::Audio* bgm_ = nullptr;

	//クリア文字
	std::unique_ptr<DaiEngine::Sprite> clearText_;
	//タイトルに戻る
	std::unique_ptr<DaiEngine::Sprite> titleBackUI_;
	//最初から始める
	std::unique_ptr<DaiEngine::Sprite> reStartUI_;

private:

	enum class Select {
		TitleBack,
		ReStrat,
	};

	Select select_ = Select::TitleBack;

	enum class UISwitch {
		On,
		Off,
	};

	UISwitch gTitleBackUISwitch_ = UISwitch::On;
	UISwitch gReStartUISwitch_ = UISwitch::Off;

};

