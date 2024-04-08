#include "SSEffectElementInitializeColor.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementInitializeColor::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.particleParam.useInitColor = true;
		color.getRange(outEmitter.particleParam.initColor, outEmitter.particleParam.initColorDiff);
	}

}
