#pragma once
#include <Siv3D.hpp>
#include "../../../Common/SSRangeValue.hpp"

namespace s3d::SpriteStudio
{
	/// @brief 速度の可変
	struct EffectElementTransformSpeed
	{
		/// @brief スピード幅
		RangeValue<float> speed{ 0.0f, 0.0f };
	};
}
