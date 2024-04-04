#include "SSEffectModel.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	const EffectNode* const EffectModel::getRootNode() const noexcept
	{
		if (0 < nodes.size())
		{
			return &(nodes[0]);
		}
		return nullptr;
	}

}
