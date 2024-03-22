#pragma once
#include <Siv3D.hpp>
#include "../../../../Common/SSTypeDefine.hpp"
#include "SSAnimationKeyFrame.hpp"

namespace s3d::SpriteStudio
{
	/// @brief アニメーションアトリビュート
	struct AnimationAttribute
	{
		/// @brief アトリビュートの種類
		AttributeKind kind;

		/// @brief アトリビュートのキーフレーム
		Array<AnimationKeyFrame> keyFrames;
	};
}
