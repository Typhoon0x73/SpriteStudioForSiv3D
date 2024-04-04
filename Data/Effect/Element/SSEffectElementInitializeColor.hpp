#pragma once
#include <Siv3D.hpp>
#include "../../../Common/SSRangeValue.hpp"

namespace s3d::SpriteStudio
{
	/// @brief 頂点カラー初期化
	struct EffectElementInitializeColor
	{
		/// @brief 色幅
		RangeValue<Color> color{ Palette::White, Palette::White };
	};
}
