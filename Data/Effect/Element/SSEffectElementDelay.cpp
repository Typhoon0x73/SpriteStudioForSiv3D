#include "SSEffectElementDelay.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementDelay::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.particleParam.delay = static_cast<float>(time);
	}
}
