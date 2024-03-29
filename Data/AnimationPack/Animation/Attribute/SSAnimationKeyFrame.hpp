#pragma once
#include <Siv3D.hpp>
#include "Value/SSAnimationAttributeValue.hpp"
#include "SSAnimationCurve.hpp"

namespace s3d::SpriteStudio
{
	/// @brief キーフレームデータ
	struct AnimationKeyFrame
	{
		/// @brief フレーム
		int32 frame{ 0 };

		/// @brief 補間の種類
		InterpolationType interpolationType{ InterpolationType::Invalid };

		/// @brief 曲線補間計算用パラメータ
		AnimationCurve curve{};

		/// @brief イーズ補間用パラメータ
		float easingRate{ 1.0f };

		/// @brief 値
		AnimationAttributeVariant attributeVariantValue{ std::monostate{} };
	};
}
