#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief エミッタ
	struct EffectEmitter;

	/// @brief 遅延
	struct EffectElementDelay
	{
		/// @brief 遅延時間
		int32 time{ 0 };

		/// @brief 初期化
		/// @param outEmitter 初期化するエミッタ
		void initialize(EffectEmitter& outEmitter) const;
	};
}
