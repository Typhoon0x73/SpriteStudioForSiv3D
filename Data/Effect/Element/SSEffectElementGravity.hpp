#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief 重力
	struct EffectElementGravity
	{
		/// @brief 重力値
		Float2 gravity{ 0.0f, -3.0f };
	};
}
