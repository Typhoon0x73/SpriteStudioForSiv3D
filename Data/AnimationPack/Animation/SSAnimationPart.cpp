#include "SSAnimationPart.hpp"

namespace s3d::SpriteStudio
{

	//================================================================================
	const AnimationAttribute* const AnimationPart::findAttribute(AttributeKind kind) const noexcept
	{
		for (const auto& it : attributes)
		{
			if (it.kind == kind)
			{
				return &it;
			}
		}
		return nullptr;
	}

}
