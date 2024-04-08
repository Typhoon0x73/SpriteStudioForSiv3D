#include "SSEffectElementGravity.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementGravity::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.particleParam.gravityOpt = gravity;
	}

}
