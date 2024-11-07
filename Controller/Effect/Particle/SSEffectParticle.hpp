#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief 
	struct EffectParticle
	{
		/// @brief 
		uint32 id{ 0 };

		/// @brief 
		int32 cycle{ 0 };

		/// @brief 存在しているか
		bool isExist{ false };

		/// @brief 生まれているか
		bool isBorn{ false };

		/// @brief 
		int64 startTime{ 0 };

		/// @brief 
		int64 endTime{ 0 };
	};
}
