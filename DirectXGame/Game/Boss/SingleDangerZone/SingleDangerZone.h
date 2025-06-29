#pragma once
#include"Boss/DangerZone/DangerZone.h"
#include"GameObject/GameObject.h"

//警告円の描画を含むクラス
class SingleDangerZone
{
public://**パブリック関数**//

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="param">パラメータ</param>
	SingleDangerZone(const DangerZoneParameters& param);
	~SingleDangerZone()=default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos"></param>
	void Init(const DangerZoneParameters& param);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 死亡チェック
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return dangerZone_->GetIsDead(); }

	/// <summary>
	/// 描画フラグ
	/// </summary>
	/// <param name="isDraw"></param>
	void SetDraw(bool isDraw) { isDraw_ = isDraw; }
private:

	//警告円
	std::unique_ptr<DangerZone>dangerZone_;

	//インスタンス化されたゲームオブジェクト
	std::unique_ptr<GameObject> gameObject_;

	//描画フラグ
	bool isDraw_ = true;
};

