#include "SSAnimation.hpp"

namespace s3d::SpriteStudio
{

	//================================================================================
	const AnimationPart* const Animation::findAnimationPart(StringView partName) const noexcept
	{
		for (const auto& it : animationParts)
		{
			if (it.name == partName)
			{
				return &it;
			}
		}
		return nullptr;
	}

}
