#include "SSEffectElementTransformRotation.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementTransformRotation::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.particleParam.useRotationTrans = true;
		outEmitter.particleParam.rotationFactor = rotationFactor;
		outEmitter.particleParam.endLifeTimePer = endLifeTimePercent;
	}

}
