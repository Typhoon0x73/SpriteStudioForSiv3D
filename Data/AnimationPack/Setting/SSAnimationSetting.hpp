#pragma once
#include <Siv3D.hpp>
#include "../../../Common/SSTypeDefine.hpp"

namespace s3d::SpriteStudio
{
	/// @brief アニメーションの設定
	struct AnimationSetting
	{
		/// @brief 再生FPS
		int32 fps;

		/// @brief フレーム数
		int32 frameCount;

		/// @brief キャンバスサイズ
		Size canvasSize;

		/// @brief キャンバスの原点。0,0 が中央。-0.5, +0.5 が左上
		Float2 pivot;

		/// @brief 開始フレーム位置
		int32 startFrame;

		/// @brief 終了フレーム位置
		int32 endFrame;

		/// @brief パーツのソートモード
		PartsSortMode sortMode;
	};
}
