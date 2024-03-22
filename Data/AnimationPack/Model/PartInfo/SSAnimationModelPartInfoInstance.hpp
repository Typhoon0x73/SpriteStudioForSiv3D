#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief インスタンスパーツの情報
	struct ModelPartInfoInstance
	{
		/// @brief 参照するアニメーションパック名
		String refAnimationPackName;

		/// @brief 参照するアニメーション名
		String refAnimationName;
	};
}
