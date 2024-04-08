#include "SSEffectElementInfiniteEmitEnabled.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementInfiniteEmitEnabled::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.emitterParam.isInfinite = true;
	}
}
