#include "SSEffectElementBasic.hpp"
#include "../../../Controller/Effect/Emitter/SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void EffectElementBasic::initialize(EffectEmitter& outEmitter) const
	{
		outEmitter.priority = priority;

		//エミッターパラメータ
		outEmitter.emitterParam.emitMax = maximumParticle;
		outEmitter.emitterParam.interval = interval;
		outEmitter.emitterParam.life = lifeTime;
		outEmitter.emitterParam.emitNum = atTimeCreate;
		outEmitter.emitterParam.particleLife = 10;
		outEmitter.emitterParam.isInfinite = false;
		outEmitter.emitterParam.loopGenerate = 0;

		//パーティクルパラメータ
		lifeSpan.getRange(outEmitter.emitterParam.particleLife, outEmitter.emitterParam.particleLifeDiff);

		outEmitter.particleParam.scale = Float2(1.0f, 1.0f);
		outEmitter.particleParam.startColor = Palette::White;
		outEmitter.particleParam.endColor = Palette::White;

		speed.getRange(outEmitter.particleParam.speed, outEmitter.particleParam.speedDiff);

		outEmitter.particleParam.angle = static_cast<float>(Math::ToRadians(angle + 90.0f));
		outEmitter.particleParam.angleVariance = static_cast<float>(ToRadians(angleVariance));

		outEmitter.particleParam.useTanAccel = false;

		//重力
		outEmitter.particleParam.gravityOpt = none;

		//オフセット
		outEmitter.particleParam.useOffset = false;
		outEmitter.particleParam.offset = Float2::Zero();
		outEmitter.particleParam.offsetDiff = Float2::Zero();

		//回転
		outEmitter.particleParam.useRotation = false;
		outEmitter.particleParam.useRotationTrans = false;

		//カラー
		outEmitter.particleParam.useInitColor = false;
		outEmitter.particleParam.useTransColor = false;

		//スケール
		outEmitter.particleParam.useInitScale = false;
		outEmitter.particleParam.useTransScale = false;

		outEmitter.particleParam.delay = 0;

		outEmitter.particleParam.usePointGravity = false;

		outEmitter.particleParam.useInitScale = false;
		outEmitter.particleParam.useAlphaFade = false;
		outEmitter.particleParam.useTransSpeed = false;
		outEmitter.particleParam.direcRotAddOpt = none;
		outEmitter.particleParam.overrideRandomSeedOpt = none;
	}

}
