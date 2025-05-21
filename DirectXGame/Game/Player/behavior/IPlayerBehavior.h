#pragma once
#include"GlobalVariable/Tree/GlobalVariableTree.h"

class Player;

//プレイヤーの状態の基底クラス
class IProtBehavior {

protected://**共通変数**//

	static Player* player_;

public://**パブリック変数**//

	//デバッグ用ツリー
	GvariTree tree_;

public://**パブリック関数**//

	/// <summary>
	/// プレイヤーをセット
	/// </summary>
	/// <param name="player"></param>
	static void SetPlayer(Player* player) { player_ = player; };

	//全てデフォルト
	IProtBehavior() = default;
	virtual ~IProtBehavior() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
};
