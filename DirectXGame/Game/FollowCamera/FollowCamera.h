#pragma once
#include"Camera.h"

//カメラ制御
class FollowCamera {

public://**パブリック関数**//

	//コンストラクタ
	FollowCamera(DaiEngine::Camera* camera, const Vector3& targetTranslation);
	//デストラクタ
	~FollowCamera() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private://**プライベート変数**//

	//カメラポインタ
	DaiEngine::Camera* camera_;

	//ターゲット座標
	const Vector3* target_;

	//カメラのオフセット座標
	Vector3 offset_ = { 0, 0, -10 };

	//追従処理フラグ
	bool isFollow_ = true;
};