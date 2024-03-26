#pragma once
#include <Siv3D.hpp>
#include "SSAnimationModelPartInfoNull.hpp"
#include "SSAnimationModelPartInfoNormal.hpp"
#include "SSAnimationModelPartInfoText.hpp"
#include "SSAnimationModelPartInfoInstance.hpp"
#include "SSAnimationModelPartInfoArmature.hpp"
#include "SSAnimationModelPartInfoEffect.hpp"
#include "SSAnimationModelPartInfoMesh.hpp"
#include "SSAnimationModelPartInfoMovenode.hpp"
#include "SSAnimationModelPartInfoConstraint.hpp"
#include "SSAnimationModelPartInfoMask.hpp"
#include "SSAnimationModelPartInfoJoint.hpp"
#include "SSAnimationModelPartInfoBonepoint.hpp"
#include "SSAnimationModelPartInfoShape.hpp"
#include "SSAnimationModelPartInfoNineslice.hpp"

namespace s3d::SpriteStudio
{
	/// @brief パーツ毎に持つ値
	using ModelPartInfoVariant = std::variant<
		std::monostate
		, ModelPartInfoNull
		, ModelPartInfoNormal
		, ModelPartInfoText
		, ModelPartInfoInstance
		, ModelPartInfoArmature
		, ModelPartInfoEffect
		, ModelPartInfoMesh
		, ModelPartInfoMovenode
		, ModelPartInfoConstraint
		, ModelPartInfoMask
		, ModelPartInfoJoint
		, ModelPartInfoBonepoint
		, ModelPartInfoShape
		, ModelPartInfoNineslice
	>;
}
