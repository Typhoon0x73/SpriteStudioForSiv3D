#include "SSEffectElementRotation.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementRotation::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.particleParam.useRotation = true;
		rotation.getRange(outEmitter.particleParam.rotation, outEmitter.particleParam.rotationDiff);
		rotation.getRange(outEmitter.particleParam.rotationAdd, outEmitter.particleParam.rotationAddDiff);
	}

}
