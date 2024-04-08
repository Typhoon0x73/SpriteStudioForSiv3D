#include "SSEffectElementTransformSize.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementTransformSize::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.particleParam.useTransScale = true;

		sizeX.getRange(outEmitter.particleParam.transScale.x, outEmitter.particleParam.transScaleRange.x);
		sizeY.getRange(outEmitter.particleParam.transScale.y, outEmitter.particleParam.transScaleRange.y);

		scaleFactor.getRange(outEmitter.particleParam.transScaleFactor, outEmitter.particleParam.transScaleFactorDiff);
	}

}
