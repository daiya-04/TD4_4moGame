#pragma once
#include"GlobalVariable/Tree/GlobalVariableTree.h"

//前方宣言
class Boss;

//ボスの状態基底クラス
class IBossBehavior {

protected://**共通変数**//

	//ボス
	static Boss* boss_;

public://**パブリック変数**//

	//デバッグ用ツリー
	GvariTree tree_;

public://パブリック関数**//

	/// <summary>
	/// ボスのポインタをセット
	/// </summary>
	/// <param name="boss"></param>
	static void SetBoss(Boss* boss) { boss_ = boss; }

	IBossBehavior() = default;
	virtual ~IBossBehavior() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

};