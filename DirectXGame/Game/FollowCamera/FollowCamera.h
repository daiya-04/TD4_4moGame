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

	enum class State {
		None,       //何もしない
		Follow,     //ターゲットを追従する
		Count
	};

	/// <summary>
	/// ステート変更
	/// </summary>
	/// <param name="state"></param>
	void SetState(State state);

	/// <summary>
	/// カメラ取得
	/// </summary>
	/// <returns></returns>
	DaiEngine::Camera* GetCamera() const { return camera_; }


private://**プライベート関数**//

	void CameraDelay(Vector3& offset);

private://**プライベート変数**//


	State state_ = State::None;

	//カメラポインタ
	DaiEngine::Camera* camera_;

	//ターゲット座標
	const Vector3* target_;

	//過去のオフセット
	Vector3 preOffset_;

	//過去の回転
	Vector3 preRotation_;

	//目標オフセット
	Vector3 targetOffset_;
	//目標回転
	Vector3 targetRotation_;
	//遷移して変わっているか否か
	bool isEsing_ = false;
	//遷移カウント
	float esingCount_ = 0.0f;

	//追従速度
	float followSpd_ = 0.1f;;

	//追従速度をあげる最大距離
	float followSpdMaxDistance_ = 10.0f;

	//最大倍率
	float maxFollowSpdMultiply_ = 2.0f;

private://**パラメータ変数**//

	//カメラのオフセット座標
	Vector3 offset_[(int)State::Count];

	//カメラの向き
	Vector3 rotation_[(int)State::Count];

	//遷移時間
	float maxEsingCount_ = 60;


};