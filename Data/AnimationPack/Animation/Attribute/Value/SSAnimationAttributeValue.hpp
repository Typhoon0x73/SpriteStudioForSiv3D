#pragma once
#include <Siv3D.hpp>
#include "SSAnimationAttributeValueCell.hpp"
#include "SSAnimationAttributeValuePartsColor.hpp"
#include "SSAnimationAttributeValueShader.hpp"
#include "SSAnimationAttributeValueVertexTranslate.hpp"
#include "SSAnimationAttributeValueUser.hpp"
#include "SSAnimationAttributeValueSignal.hpp"
#include "SSAnimationAttributeValueInstance.hpp"
#include "SSAnimationAttributeValueEffect.hpp"
#include "SSAnimationAttributeValueDeform.hpp"
#include "SSAnimationAttributeValueAudio.hpp"
#include "SSAnimationAttributeValueTextureChange.hpp"

namespace s3d::SpriteStudio
{
	/// @brief アトリビュート値
	using AnimationAttributeVariant = std::variant<
		std::monostate
		, String
		, int32
		, float
		, bool
		, AttributeValueCell
		, AttributeValuePartsColor
		, AttributeValueShader
		, AttributeValueVertexTranslate
		, AttributeValueUser
		, AttributeValueSignal
		, AttributeValueInstance
		, AttributeValueEffect
		, AttributeValueDeform
		, AttributeValueAudio
		, AttributeValueTextureChange
	>;
}
