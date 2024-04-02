#include "SSAnimationPack.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	const Animation* const AnimationPack::getSetupAnimation() const
	{
		if (setupIndex < 0 or static_cast<int32>(animations.size()) <= setupIndex)
		{
			return nullptr;
		}
		return &(animations[setupIndex]);
	}

	//================================================================================
	const Animation* const AnimationPack::findAnimation(StringView animationName) const
	{
		for (const auto& it : animations)
		{
			if (it.name == animationName)
			{
				return &it;
			}
		}
		return nullptr;
	}

}
