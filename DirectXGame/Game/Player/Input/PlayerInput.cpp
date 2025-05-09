#include "PlayerInput.h"

PlayerInput::PlayerInput()
{
	//入力のインスタンス取得
	input_ = DaiEngine::Input::GetInstance();
}

Vector3 PlayerInput::GetMoveInput()
{
	//回答する変数生成
	Vector3 ans;

	//矢印キーの入力チェック
	ans.x =-(float)input_->PushKey(DIK_LEFT) + (float)input_->PushKey(DIK_RIGHT);
	ans.z = (float) input_->PushKey(DIK_UP)	 - (float)input_->PushKey(DIK_DOWN);


	//コントローラー入力取得
	ans += input_->GetMoveXZ();

	if (ans.Length() != 0)
	{
		//正規化
		ans=ans.Normalize();
	}

	//返却
	return ans;
}

bool PlayerInput::GetInput(Type type)
{
	switch (type)
	{
	case PlayerInput::Roll:
		return RollInput();
	case PlayerInput::Attack:
		return AttackInput();
	case PlayerInput::Count:
		break;
	default:
		break;
	}

	//
	return false;
}

bool PlayerInput::RollInput()
{
	//入力チェック変数生成と初期化
	int ans=0;

	//キー入力チェック
	ans = input_->TriggerKey(DIK_LSHIFT);

	//コントローラー入力チェック
	ans += input_->TriggerButton(DaiEngine::Input::Button::B);

	//返却
	return (bool)ans;
}

bool PlayerInput::AttackInput()
{
	//入力チェック変数生成と初期化
	int ans = 0;

	//キー入力チェック
	ans = input_->TriggerKey(DIK_SPACE);

	//コントローラー入力チェック
	ans += input_->TriggerButton(DaiEngine::Input::Button::X);

	//返却
	return (bool)ans;
}
