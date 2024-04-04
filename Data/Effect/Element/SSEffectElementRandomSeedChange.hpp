#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief ランダムシード値変更
	struct EffectElementRandomSeedChange
	{
		/// @brief シード値
		int32 seed{ 0 };
	};
}
