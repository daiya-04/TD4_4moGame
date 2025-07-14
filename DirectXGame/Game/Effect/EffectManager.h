#pragma once
#include "Effect.h"
#include <map>


class EffectManager {
public:

	static EffectManager* GetInstance();

	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const DaiEngine::Camera& camera);
	/// <summary>
	/// 再生開始
	/// </summary>
	/// <param name="pos"></param>
	void Start(const std::string& effectName, const Vector3& pos);
	void Start(const std::string& effectName, const Vector3& pos, float angle);
	/// <summary>
	/// エフェクトを追加
	/// </summary>
	void AddEffect(const std::string& fileName, const std::string& modelName = "");
	/// <summary>
	/// 全エフェクト消去
	/// </summary>
	void ClearEffects() { effects_.clear(); }

private:

	std::map<std::string, std::unique_ptr<Effect>> effects_;


private:

	EffectManager() = default;
	~EffectManager() = default;
	EffectManager(const EffectManager&) = delete;
	EffectManager& operator=(const EffectManager&) = delete;

};

