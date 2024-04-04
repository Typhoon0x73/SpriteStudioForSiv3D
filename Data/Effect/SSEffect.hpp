#pragma once
#include <Siv3D.hpp>
#include "SSEffectModel.hpp"

namespace s3d::SpriteStudio
{
	/// @brief エフェクト
	struct Effect
	{
		/// @brief 名前
		String name{ U"" };

		/// @brief モデル
		EffectModel model{};
	};
}
