#include "SSEffectElementTangentialAcceleration.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementTangentialAcceleration::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.particleParam.useTanAccel = true;
		acceleration.getRange(outEmitter.particleParam.tangentialAccel, outEmitter.particleParam.tangentialAccelDiff);
	}

}
