#include "SSEffectElementPointGravity.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementPointGravity::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.particleParam.usePointGravity = true;

		outEmitter.particleParam.gravityPos = position;
		outEmitter.particleParam.gravityPower = power;
	}

}
