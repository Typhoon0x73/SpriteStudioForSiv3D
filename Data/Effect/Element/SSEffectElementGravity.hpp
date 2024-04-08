#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief エミッタ
	struct EffectEmitter;

	/// @brief 重力
	struct EffectElementGravity
	{
		/// @brief 重力値
		Float2 gravity{ 0.0f, -3.0f };

		/// @brief 初期化
		/// @param outEmitter 初期化するエミッタ
		void initialize(EffectEmitter& outEmitter) const;
	};
}
