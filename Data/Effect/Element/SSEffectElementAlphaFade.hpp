#pragma once
#include <Siv3D.hpp>
#include "../../../Common/SSRangeValue.hpp"

namespace s3d::SpriteStudio
{
	/// @brief アルファによるフェード
	struct EffectElementAlphaFade
	{
		/// @brief 
		RangeValue<float> dispRange{ 25.0f, 75.0f };
	};
}
