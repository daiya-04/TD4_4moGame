#pragma once
///---------------------------------------------------------------------------------------------
// 
// 地面から炎攻撃の管理
// 
///---------------------------------------------------------------------------------------------

#include "GroundFlare.h"
#include "BaseBossAttackManager.h"
#include "BurnScar.h"

#include <array>

class GroundFlareManager : public BaseBossAttackManager {
public:
	~GroundFlareManager() override;
	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const DaiEngine::Camera& camera) override;
	/// <summary>
	/// パーティクル描画
	/// </summary>
	/// <param name="camera"></param>
	void DrawParticle(const DaiEngine::Camera& camera) override;
	/// <summary>
	/// 焼け跡描画
	/// </summary>
	/// <param name="camera"></param>
	void DrawBurnScars(const DaiEngine::Camera& camera);
	/// <summary>
	/// ターゲットセット
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const Vector3* target) override { target_ = target; }
	/// <summary>
	/// 攻撃開始
	/// </summary>
	void AttackStart();
	/// <summary>
	/// 攻撃が終了したか
	/// </summary>
	/// <returns>攻撃が終了した瞬間true、それ以外はfalse</returns>
	bool AttackFinish() const { return (!IsAttack() && preIsAttack_); }
	/// <summary>
	/// 攻撃が始まったか
	/// </summary>
	/// <returns>攻撃が開始した瞬間true、それ以外はfalse</returns>
	bool FireStartFlag() const { return(IsFire() && !preIsFire_); }
	/// <summary>
	/// 攻撃中か
	/// </summary>
	/// <returns></returns>
	bool IsAttack() const;
	/// <summary>
	/// 発射中か
	/// </summary>
	/// <returns></returns>
	bool IsFire() const;

private:
	//発車する炎の総数
	static const uint32_t kGroundFlareNum_ = 6;

	std::array<std::unique_ptr<GroundFlare>, kGroundFlareNum_> groundFlares_;

	std::array<std::unique_ptr<BurnScar>, kGroundFlareNum_> burnScares_;

	//中心座標
	DaiEngine::WorldTransform worldtransform_;

	//攻撃先（ターゲット）
	const Vector3* target_ = nullptr;
	//発車場所のオフセット
	std::array<Vector3, kGroundFlareNum_> offsets_;
	//オフセットの距離
	const float offsetLength_ = 4.5f;

	//1フレーム前の攻撃フラグ
	bool preIsAttack_ = false;
	//1フレーム前の発射フラグ
	bool preIsFire_ = false;

};

