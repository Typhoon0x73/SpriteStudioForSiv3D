#include "SSEffectElementRandomSeedChange.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"
#include "../../../Common/SSTypeDefine.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementRandomSeedChange::initialize(EffectEmitter& outEmitter) const
	{
		auto overrideRandomSeed = seed + SeedMagic;
		outEmitter.particleParam.overrideRandomSeedOpt = overrideRandomSeed;
		outEmitter.emitterSeed = overrideRandomSeed;
	}
}
