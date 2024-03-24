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
		int32 frame;

		/// @brief 補間の種類
		InterpolationType interpolationType;

		/// @brief 曲線補間計算用パラメータ
		AnimationCurve curve;

		/// @brief 値
		AnimationAttributeVariant attributeVariantValue;
	};
}
