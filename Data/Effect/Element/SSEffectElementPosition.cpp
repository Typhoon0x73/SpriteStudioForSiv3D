#include "SSEffectElementPosition.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementPosition::initialize(EffectEmitter& outEmitter) const
	{
		offsetX.getRange(outEmitter.particleParam.offset.x, outEmitter.particleParam.offsetDiff.x);
		offsetX.getRange(outEmitter.particleParam.offset.y, outEmitter.particleParam.offsetDiff.y);
	}

}
