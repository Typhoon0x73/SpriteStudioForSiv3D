#include "SSEffectElementAlphaFade.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementAlphaFade::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.particleParam.useAlphaFade = true;
		outEmitter.particleParam.alphaFade = dispRange.getLeftValue();
		outEmitter.particleParam.alphaFadeMax = dispRange.getRightValue();
	}
}
