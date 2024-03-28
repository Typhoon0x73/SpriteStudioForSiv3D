#pragma once
#include <Siv3D.hpp>
#include "Attribute/SSAnimationAttribute.hpp"

namespace s3d::SpriteStudio
{
	/// @brief パーツ毎のアニメーションキーフレームデータ
	struct AnimationPart
	{
		/// @brief パーツ名
		String name{ U"" };

		/// @brief アトリビュート
		Array<AnimationAttribute> attributes{};

		/// @brief 指定された種類のアトリビュートを探します。
		/// @param kind 探したい種類
		/// @return 見つけた場合、アトリビュートのポインタが返ります。 それ以外nullptr
		[[nodiscard]]
		const AnimationAttribute* const findAttribute(AttributeKind kind) const noexcept;
	};
}
