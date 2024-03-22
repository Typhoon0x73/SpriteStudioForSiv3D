#pragma once
#include <Siv3D.hpp>
#include "Value/SSAttributeValue.hpp"

namespace s3d::SpriteStudio
{
	/// @brief キーフレームデータ
	struct AnimationKeyFrame
	{
		/// @brief フレーム
		int32 frame;

		/// @brief 補間の種類
		InterpolationType interpolationType;

		/// @brief 値
		AnimationAttributeVariant attributeVariantValue;
	};
}
