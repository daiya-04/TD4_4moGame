#pragma once
///---------------------------------------------------------------------------------------------
//
// InstancingObject3dGraphicsPipeline
// インスタンシング3Dオブジェクト用のパイプラインステートの生成
//
///---------------------------------------------------------------------------------------------


#include "BaseGraphicsPipeline.h"

namespace DaiEngine {
	class InstancingObject3dGraphicsPipeline : public BaseGraphicsPipeline {
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Init() override;
		/// <summary>
		/// 描画前
		/// </summary>
		/// <param name="blendMode"></param>
		void preDraw(BlendMode blendMode = BlendMode::kAlpha) override;

	};
}
