#pragma once
#include <Siv3D.hpp>
#include "../../../Common/SSRangeValue.hpp"

namespace s3d::SpriteStudio
{
	/// @brief エミッタ
	struct EffectEmitter;

	/// @brief アルファによるフェード
	struct EffectElementAlphaFade
	{
		/// @brief 
		RangeValue<float> dispRange{ 25.0f, 75.0f };

		/// @brief 初期化
		/// @param outEmitter 初期化するエミッタ
		void initialize(EffectEmitter& outEmitter) const;
	};
}
