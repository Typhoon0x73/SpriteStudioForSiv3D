#pragma once
#include <Siv3D.hpp>
#include "../../../Common/SSRangeValue.hpp"

namespace s3d::SpriteStudio
{
	/// @brief エミッタ
	struct EffectEmitter;

	/// @brief パーティクルを構成する基本の値
	struct EffectElementBasic
	{
		/// @brief 最大パーティクル数
		int32 maximumParticle{ 50 };

		/// @brief 速度
		RangeValue<float> speed{ 5.0f, 5.0f };

		/// @brief 生存間隔
		RangeValue<int32> lifeSpan{ 30, 30 };

		/// @brief 
		float angle{ 0.0f };

		/// @brief 
		float angleVariance{ 45.0f };

		/// @brief 間隔
		int32 interval{ 1 };

		/// @brief 生存時間
		int32 lifeTime{ 30 };

		/// @brief 
		int32 atTimeCreate{ 1 };

		/// @brief 優先度
		int32 priority{ 64 };

		/// @brief 初期化
		/// @param outEmitter 初期化するエミッタ
		void initialize(EffectEmitter& outEmitter) const;
	};
}
