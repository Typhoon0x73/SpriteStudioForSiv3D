#include "SSEffectElementInitializeSize.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementInitializeSize::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.particleParam.useInitScale = true;

		sizeX.getRange(outEmitter.particleParam.scale.x, outEmitter.particleParam.scaleRange.x);
		sizeY.getRange(outEmitter.particleParam.scale.y, outEmitter.particleParam.scaleRange.y);

		scaleFactor.getRange(outEmitter.particleParam.scaleFactor, outEmitter.particleParam.scaleFactorDiff);
	}

}

