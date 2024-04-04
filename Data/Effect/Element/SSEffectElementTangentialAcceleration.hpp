#pragma once
#include <Siv3D.hpp>
#include "../../../Common/SSRangeValue.hpp"

namespace s3d::SpriteStudio
{
	/// @brief 接戦加速度を与える
	struct EffectElementTangentialAcceleration
	{
		/// @brief 加速幅
		RangeValue<float> acceleration{ 0.0f, 0.0f };
	};
}
