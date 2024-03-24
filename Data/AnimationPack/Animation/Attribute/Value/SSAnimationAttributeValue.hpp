#pragma once
#include <Siv3D.hpp>
#include "SSAnimationAttributeValueInstance.hpp"

namespace s3d::SpriteStudio
{
	/// @brief アトリビュート値
	using AnimationAttributeVariant = std::variant<
		std::monostate
		, String
		, int32
		, float
		, bool
		, AttributeValueInstance
	>;
}
