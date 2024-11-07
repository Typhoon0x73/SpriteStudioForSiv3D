#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief 
	struct EffectParticleDrawData
	{
		/// @brief 
		int32 id{ -1 };

		/// @brief 
		int32 parentId{ -1 };

		/// @brief 生成された開始時間
		int32 startTime{ 0 };

		/// @brief 
		int32 lifeTime{ 0 };

		/// @brief 
		Mat4x4 matrix{};

		/// @brief 
		Buffer2D drawBuffer{};

		/// @brief 
		Float2 pos{ Float2::Zero() };

		/// @brief 
		float rotation{ 0.0f };

		/// @brief 
		float direction{ 0.0f };

		/// @brief 
		Color color{ Palette::White };

		/// @brief 
		Float2 scale{ Float2::One() };
	};
}
