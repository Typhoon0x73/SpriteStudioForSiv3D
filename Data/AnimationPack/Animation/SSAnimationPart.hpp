#pragma once
#include <Siv3D.hpp>
#include "Attribute/SSAnimationAttribute.hpp"

namespace s3d::SpriteStudio
{
	/// @brief パーツ毎のアニメーションキーフレームデータ
	struct AnimationPart
	{
		/// @brief パーツ名
		String name;

		/// @brief アトリビュート
		Array<AnimationAttribute> attributes;
	};
}
