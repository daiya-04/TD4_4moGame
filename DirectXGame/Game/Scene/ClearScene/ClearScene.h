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

#include "ClearLogo.h"
#include "CrossMark.h"


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

	DaiEngine::Audio* choiceSE_ = nullptr;
	DaiEngine::Audio* doneSE_ = nullptr;

	//クリア文字
	std::unique_ptr<ClearLogo> clearText_;
	//タイトルに戻る
	std::unique_ptr<DaiEngine::Sprite> titleBackUI_;
	//最初から始める
	std::unique_ptr<DaiEngine::Sprite> reStartUI_;

	std::map<std::string, std::unique_ptr<DaiEngine::Sprite>> bossUIs_;
	float bossUIScale_ = 1.0f;

	std::vector<std::unique_ptr<CrossMark>> crossMarks_;

private:

	enum class Select {
		TitleBack,
		ReStart,
	};

	std::vector<Select> order_{
		Select::TitleBack,
		Select::ReStart,
	};

	std::map<Select, std::function<void()>> onSelect_ = {
		{Select::TitleBack, [this]() {ToTitle(); }},
		{Select::ReStart, [this]() {ToGame(); }},
	};

	Select select_ = Select::TitleBack;

	enum class UISwitch {
		On,
		Off,
	};

	UISwitch gTitleBackUISwitch_ = UISwitch::On;
	UISwitch gReStartUISwitch_ = UISwitch::Off;

private:

	void MenuInput();

	void ToGame();
	void ToTitle();

private:

	enum EffectState {
		BossDefeat,
		LogoApper,
		MenuReady,
	};

	EffectState state_ = EffectState::BossDefeat;
	std::optional<EffectState> stateRequest_ = std::nullopt;

	std::map<EffectState, std::function<void()>> stateInitTable_ = {
		{EffectState::BossDefeat, [this]() {BossDefeatInit(); }},
		{EffectState::LogoApper, [this]() {LogoApperInit(); }},
		{EffectState::MenuReady, [this]() {MenuReadyInit(); }},
	};

	std::map<EffectState, std::function<void()>> stateUpdateTable_ = {
		{EffectState::BossDefeat, [this]() {BossDefeatUpdate(); }},
		{EffectState::LogoApper, [this]() {LogoApperUpdate(); }},
		{EffectState::MenuReady, [this]() {MenuReadyUpdate(); }},
	};

	float timer_ = 0.0f;
	const float kDeltaTime_ = 1.0f / 60.0f;
	float stampInterval_ = 1.0f;
	size_t stampedIndex_ = 0;

	struct StampStartData {
		float speed;
		float startScale;
	};

	std::vector<StampStartData> stampStartDatas_{
		{1.0f / 30.0f, 2.0f},
		{1.0f / 30.0f, 2.0f},
		{1.0f / 60.0f, 2.7f},
	};
	
private:

	void BossDefeatInit();
	void BossDefeatUpdate();

	void LogoApperInit();
	void LogoApperUpdate();

	void MenuReadyInit();
	void MenuReadyUpdate();

};

