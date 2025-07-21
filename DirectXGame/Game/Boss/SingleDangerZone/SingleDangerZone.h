#pragma once
#include"Boss/DangerZone/IDangerZone/IDangerZone.h"
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
	/// 描画フラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDraw() { return isDraw_; }

	/// <summary>
	/// 描画フラグ
	/// </summary>
	/// <param name="isDraw"></param>
	void SetDraw(bool isDraw) { isDraw_ = isDraw; }
private:

	//警告円
	std::unique_ptr<IDangerZone>dangerZone_;

	//インスタンス化されたゲームオブジェクト
	std::unique_ptr<GameObject> gameObject_;

	Vector3 pPos_ = { 0,0,0 };

	//描画フラグ
	bool isDraw_ = true;
};

