#pragma once
///---------------------------------------------------------------------------------------------
// 
// ゲームシーン
// 
///---------------------------------------------------------------------------------------------


#include "IScene.h"
#include <memory>
#include <list>
#include <vector>
#include <optional>
#include <map>
#include <string>
#include <functional>

#include "Sprite.h"
#include "Object3d.h"
#include "SkinningObject.h"
#include "InstancingObject3d.h"
#include "Particle.h"
#include "GPUParticle.h"
#include "Camera.h"
#include "WorldTransform.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "SkyBox.h"
#include "Audio.h"

#pragma region キクチ
#include"Player/Player.h"
#include"GlobalVariable/Manager/GlobalVaribleManager.h"
#include"FollowCamera/FollowCamera.h"
#include"../../Boss/SpawnManager/BossSpawnManager.h"
#pragma endregion
#include"Field/Field.h"

#include "PlayerAttackEffect.h"

//ゲームシーンクラス
class GameScene : public DaiEngine::IScene {
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
	~GameScene()override;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

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

private://**キクチ
	
	//デバッグパラメータマネージャ
	GlobalVariableManager* globalVariableManager_ = nullptr;

	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<PlayerAttackEffect> playerAttackEffect_;

	//ボス
	std::unique_ptr<BossSpawnManager> bossSpawnManager_ = nullptr;

	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
	
	//地面
	std::unique_ptr<Field> field_ = nullptr;

private:

	DaiEngine::Audio* bgm_ = nullptr;

	//UI（とりあえず雑に）
	std::map<std::string, std::unique_ptr<DaiEngine::Sprite>> uis_;

};

