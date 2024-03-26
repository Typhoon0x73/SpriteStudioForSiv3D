#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief オーディオパラメータ
	struct AttributeValueAudio
	{
		/// @brief 
		int32 id{ -1 };

		/// @brief 
		String name{ U"" };

		/// @brief 
		int32 loopNum{ 1 };
	};
}
