#include "SSEffectElementTransformColor.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementTransformColor::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.particleParam.useTransColor = true;
		color.getRange(outEmitter.particleParam.transColor, outEmitter.particleParam.transColorDiff);
	}
}
