#pragma once
///---------------------------------------------------------------------------------------------
//
// Framework
//
///---------------------------------------------------------------------------------------------


#include "WinApp.h"
#include "DirectXCommon.h"
#include "SceneManager.h"
#include <memory>

namespace DaiEngine {
	//フレームワーククラス
	class DSFramework {
	public:

		virtual ~DSFramework() = default;
		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Init();
		/// <summary>
		/// 更新
		/// </summary>
		virtual void Update();
		/// <summary>
		/// 描画
		/// </summary>
		virtual void Draw() = 0;
		/// <summary>
		/// 解消
		/// </summary>
		virtual void Finalize();
		/// <summary>
		/// ゲーム全体
		/// </summary>
		void Run();

	};
}
