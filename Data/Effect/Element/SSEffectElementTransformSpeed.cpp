#include "SSEffectElementTransformSpeed.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementTransformSpeed::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.particleParam.useTransSpeed = true;
		speed.getRange(outEmitter.particleParam.transSpeed, outEmitter.particleParam.transSpeedDiff);
	}

}
