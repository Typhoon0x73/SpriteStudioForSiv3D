#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	struct Label
	{
		/// @brief ラベル名
		String name;

		/// @brief ラベルが設置されたフレーム
		int32 frame;
	};
}
