#pragma once
#include "Sprite.h"
#include "Boss/BossType/BossType.h"
#include"../IBoss/IBoss.h"

#include <vector>
#include<memory>


struct GaugeSetData {
	Vector2 freamPos{};
	Vector2 freamSize{};
	Vector2 gaugePos{};
	Vector2 gaugeSize{};
};

class BossGauge {
public:

	BossGauge();
	~BossGauge() = default;

	void Init();

	void Update(const int HP_, const int maxHP);

	void Draw();

	void SetData(const GaugeSetData& data);

private:

	std::unique_ptr<DaiEngine::Sprite> hpFream_;
	std::unique_ptr<DaiEngine::Sprite> hpGauge_;

	float curPer_ = 1.0f;
	float percent_ = 1.0f;
	Vector2 gaugeSize_ = {};

};

class BossUI {
private:

	struct IconSetData {
		uint32_t texHandle = 0;
		Vector2 pos{};
	};

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BossUI();
	~BossUI()=default;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="HP_"></param>
	/// <param name="maxHP"></param>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetBossData(const std::vector<std::unique_ptr<IBoss>>& bosses);

private:

	std::vector<IBoss*> bosses_;
	std::vector<std::unique_ptr<BossGauge>> gauge_;
	std::vector<GaugeSetData> gaugeSetData_;

	//UI
	std::vector<std::unique_ptr<DaiEngine::Sprite>> icons_;
	std::vector<IconSetData> iconSetData_;

};