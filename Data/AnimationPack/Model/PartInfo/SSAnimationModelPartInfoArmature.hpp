#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief ボーンパーツの情報
	struct ModelPartInfoArmature
	{
		/// @brief ボーンの位置
		Float2 bonePosition;

		/// @brief ボーンの回転(degree)
		float boneRotation;
	};
}
