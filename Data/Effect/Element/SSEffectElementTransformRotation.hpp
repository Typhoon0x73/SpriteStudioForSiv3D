#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief エミッタ
	struct EffectEmitter;

	/// @brief 
	struct EffectElementTransformRotation
	{
		/// @brief 回転係数
		float rotationFactor{ 0.0f };

		/// @brief 寿命終了の割合
		float endLifeTimePercent{ 75.0f };

		/// @brief 初期化
		/// @param outEmitter 初期化するエミッタ
		void initialize(EffectEmitter& outEmitter) const;
	};
}
