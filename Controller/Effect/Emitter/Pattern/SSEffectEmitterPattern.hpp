#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief 
	struct EffectEmitterPattern
	{
		/// @brief 
		uint32 uniqueId{ 0 };

		/// @brief 
		int32 life{ 0 };

		/// @brief 
		int32 cycle{ 0 };
	};
}
