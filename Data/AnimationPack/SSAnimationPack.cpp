#include "SSAnimationPack.hpp"

namespace s3d::SpriteStudio
{

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
