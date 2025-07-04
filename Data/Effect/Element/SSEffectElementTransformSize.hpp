﻿#pragma once
#include <Siv3D.hpp>
#include "../../../Common/SSRangeValue.hpp"

namespace s3d::SpriteStudio
{
	/// @brief 
	struct EffectElementTransformSize
	{
		/// @brief 横幅
		RangeValue<float> sizeX{ 1.0f, 1.0f };

		/// @brief 立幅
		RangeValue<float> sizeY{ 1.0f, 1.0f };

		/// @brief 拡縮係数
		RangeValue<float> scaleFactor{ 1.0f, 1.0f };
	};
}
