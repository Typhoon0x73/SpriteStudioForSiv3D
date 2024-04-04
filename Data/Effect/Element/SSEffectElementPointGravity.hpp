#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief 
	struct EffectElementPointGravity
	{
		/// @brief 座標
		Float2 position{ 0.0f, 0.0f };

		/// @brief 力
		float  power{ 0.0f };
	};
}
