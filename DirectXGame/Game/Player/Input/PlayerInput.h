#pragma once
#include"Input.h"

//プレイヤー入力をまとめるクラス
class PlayerInput
{
public://**パブリック変数**//

	enum Type {
		Roll,
		Attack,
		Count
	};


public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerInput();
	~PlayerInput()=default;

	/// <summary>
	/// 入力移動取得
	/// </summary>
	/// <returns>入力方向</returns>
	Vector3 GetMoveInput();

	/// <summary>
	/// 入力チェック
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	bool GetInput(Type type);

private://**プライベート関数**//

	/// <summary>
	/// 回避入力関数
	/// </summary>
	bool RollInput();

	/// <summary>
	/// 攻撃入力関数
	/// </summary>
	/// <returns></returns>
	bool AttackInput();

private://**プライベート変数**//

	//入力クラス
	DaiEngine::Input* input_=nullptr;


};