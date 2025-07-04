﻿#pragma once
#include <Siv3D.hpp>
#include "../../../Common/SSRangeValue.hpp"

namespace s3d::SpriteStudio
{
	/// @brief 回転
	struct EffectElementRotation
	{
		/// @brief 回転幅
		RangeValue<float> rotation{ 0.0f, 0.0f };

		/// @brief 加算回転幅
		RangeValue<float> rotationAdd{ 0.0f, 0.0f };
	};
}
