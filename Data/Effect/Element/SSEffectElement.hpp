#pragma once
#include <Siv3D.hpp>
#include "SSEffectElementBasic.hpp"
#include "SSEffectElementRandomSeedChange.hpp"
#include "SSEffectElementDelay.hpp"
#include "SSEffectElementGravity.hpp"
#include "SSEffectElementPosition.hpp"
#include "SSEffectElementRotation.hpp"
#include "SSEffectElementTransformRotation.hpp"
#include "SSEffectElementTransformSpeed.hpp"
#include "SSEffectElementTangentialAcceleration.hpp"
#include "SSEffectElementInitializeColor.hpp"
#include "SSEffectElementTransformColor.hpp"
#include "SSEffectElementAlphaFade.hpp"
#include "SSEffectElementInitializeSize.hpp"
#include "SSEffectElementTransformSize.hpp"
#include "SSEffectElementPointGravity.hpp"
#include "SSEffectElementTurnToDirectionEnabled.hpp"
#include "SSEffectElementInfiniteEmitEnabled.hpp"

namespace s3d::SpriteStudio
{
	/// @brief エフェクト効果値
	using EffectElementVariant = std::variant<
		std::monostate
		, EffectElementBasic
		, EffectElementRandomSeedChange
		, EffectElementDelay
		, EffectElementGravity
		, EffectElementPosition
		, EffectElementRotation
		, EffectElementTransformRotation
		, EffectElementTransformSpeed
		, EffectElementTangentialAcceleration
		, EffectElementInitializeColor
		, EffectElementTransformColor
		, EffectElementAlphaFade
		, EffectElementInitializeSize
		, EffectElementTransformSize
		, EffectElementPointGravity
		, EffectElementTurnToDirectionEnabled
		, EffectElementInfiniteEmitEnabled
	>;
}
