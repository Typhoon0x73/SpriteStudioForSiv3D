#pragma once
#include <Siv3D.hpp>
#include "../../../Common/SSRangeValue.hpp"

namespace s3d::SpriteStudio
{
	/// @brief 頂点カラー制御
	struct EffectElementTransformColor
	{
		/// @brief 色幅
		RangeValue<Color> color{ Palette::White, Palette::White };
	};
}
