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

	//================================================================================
	const AnimationKeyFrame* const AnimationAttribute::findLeftKey(int32 frame) const noexcept
	{
		if (keyFrames.isEmpty())
		{
			return nullptr;
		}
		for (auto it = keyFrames.crbegin(); it != keyFrames.crend(); ++it)
		{
			if (it->frame <= frame)
			{
				return &(*it);
			}
		}
		// データはあるのに見つからないらしい。
		return nullptr;
	}

	//================================================================================
	const AnimationKeyFrame* const AnimationAttribute::findRightKey(int32 frame) const noexcept
	{
		if (keyFrames.isEmpty())
		{
			return nullptr;
		}
		for (auto it = keyFrames.cbegin(); it != keyFrames.cend(); ++it)
		{
			if (frame < it->frame)
			{
				return &(*it);
			}
		}
		// データはあるのに見つからないらしい。
		return nullptr;
	}

}
