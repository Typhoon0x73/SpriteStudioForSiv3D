#include "SSEffectElementTurnToDirectionEnabled.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void s3d::SpriteStudio::EffectElementTurnToDirectionEnabled::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.particleParam.direcRotAddOpt = rotation;
	}

}
