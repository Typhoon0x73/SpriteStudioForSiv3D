﻿#pragma once
#include <Siv3D.hpp>
#include "../../../Common/SSRangeValue.hpp"

namespace s3d::SpriteStudio
{
	/// @brief 座標
	struct EffectElementPosition
	{
		/// @brief 左右幅
		RangeValue<float> offsetX{ 0.0f, 0.0f };

		/// @brief 上下幅
		RangeValue<float> offsetY{ 0.0f, 0.0f };
	};
}
