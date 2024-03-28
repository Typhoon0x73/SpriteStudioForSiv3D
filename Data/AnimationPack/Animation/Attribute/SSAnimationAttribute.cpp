#include "SSAnimationAttribute.hpp"

namespace s3d::SpriteStudio
{

	//================================================================================
	const AnimationKeyFrame* const AnimationAttribute::getFirstKey() const noexcept
	{
		if (keyFrames.isEmpty())
		{
			return nullptr;
		}
		return &(keyFrames[0]);
	}

}
