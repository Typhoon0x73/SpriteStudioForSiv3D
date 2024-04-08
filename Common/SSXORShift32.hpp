#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	struct XORShift32
	{
		/// @brief 乱数シード値
		uint32 seed{ 0 };

		/// @brief 乱数の生成
		inline uint32 generateUint32();

		/// @brief 乱数の生成
		inline float generateFloat32();
	};

#include "SSXORShift32.ipp"
}
