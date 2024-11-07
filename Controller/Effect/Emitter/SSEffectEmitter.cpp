#include "SSEffectEmitter.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	void s3d::SpriteStudio::EffectEmitter::preCalculate()
	{
		random.seed = emitterSeed;
		emitPatterns.clear();
		offsetPatterns.clear();

		const int32 emitMax = emitterParam.emitMax;
		if (0 < emitMax)
		{
			particleDrawInfos.clear();
			particleDrawInfos.resize(static_cast<size_t>(emitMax));
			if (particles.isEmpty())
			{
				particles.resize(static_cast<size_t>(emitMax));
			}
		}
		for (uint32 i = 0; i < emitMax; i++)
		{
			particles[i].id = i;
		}

		// 最低１保証
		if (emitterParam.emitNum < 1)
		{
			emitterParam.emitNum = 1;
		}

		const float totalInterval = static_cast<float>(emitMax * emitterParam.interval);
		const int32 cycle = static_cast<int>((totalInterval / static_cast<float>(emitterParam.emitNum)) + 0.5f);

		int32 extendSize = Min(emitMax * EffectLifeExtendScale, EffectLifeExtendScale);

		int32 shot = 0;
		int32 offset = static_cast<int32>(particleParam.delay);
		for (int32 i = 0; i < emitMax; i++)
		{
			if (emitterParam.emitNum <= shot)
			{
				shot = 0;
				offset += emitterParam.interval;
			}
			offsetPatterns.emplace_back(offset);
			shot++;
		}

		for (int32 i = 0; i < extendSize; i++)
		{
			EffectEmitterPattern emitterPattern;
			emitterPattern.uniqueId = static_cast<uint32>(i);
			emitterPattern.life = emitterParam.particleLife + static_cast<int32>(emitterParam.particleLifeDiff * random.generateFloat32());
			emitterPattern.cycle = cycle;

			if (cycle < emitterPattern.life)
			{
				emitterPattern.cycle = emitterPattern.life;
			}

			emitPatterns.emplace_back(emitterPattern);
		}

		seeds.clear();

		particleListBufferSize = static_cast<size_t>(emitMax);
		random.seed = emitterSeed;
		seedTableLength = particleListBufferSize * 3;
		seeds.resize(seedTableLength);
		//各パーティクルＩＤから参照するシード値をテーブルとして作成する
		Array<uint32>& seedsRaw = seeds;
		for (size_t i = 0; i < seedTableLength; i++)
		{
			seedsRaw[i] = random.generateUint32();
		}
	}

	//================================================================================
	void EffectEmitter::update(int32 targetFrame, int32 offset)
	{
		const size_t offsetNum = offsetPatterns.size();
		const size_t emitNum = emitPatterns.size();
		const int32 slide = offset * SeedMagic;

		auto& particlesRaw = particles;
		for (size_t i = 0; i < offsetNum; i++)
		{
			const size_t slideIndex = (i + slide) % emitNum;

			const EffectEmitterPattern* pTarget = &emitPatterns[slideIndex];

			const int32 tmpTime = targetFrame - offsetPatterns[i];

			particlesRaw[i].isExist = false;
			particlesRaw[i].isBorn = false;

			if (pTarget->cycle != 0)
			{
				int32 loopCount = tmpTime / pTarget->cycle;
				int32 cycleTop = loopCount * pTarget->cycle;

				particlesRaw[i].cycle = loopCount;

				particlesRaw[i].startTime = static_cast<int64>(cycleTop + offsetPatterns[i]);
				particlesRaw[i].endTime = particlesRaw[i].startTime + pTarget->life;

				if (particlesRaw[i].startTime <= static_cast<int64>(targetFrame)
					and static_cast<int64>(targetFrame) < particlesRaw[i].endTime)
				{
					particlesRaw[i].isExist = true;
					particlesRaw[i].isBorn = true;
				}

				if (not(emitterParam.isInfinite))
				{
					// エミッターが終了している
					if (emitterParam.life <= particlesRaw[i].startTime) 
					{
						// 作られてない
						particlesRaw[i].isExist = false;

						// 最終的な値に計算し直し <-事前計算しておくといいかも？
						const int32 tmpTime2 = emitterParam.life - offsetPatterns[i];
						loopCount = tmpTime2 / pTarget->cycle;

						cycleTop = loopCount * pTarget->cycle;

						particlesRaw[i].startTime = static_cast<int64>(cycleTop + offsetPatterns[i]);

						particlesRaw[i].endTime = particlesRaw[i].startTime + pTarget->life;
						particlesRaw[i].isBorn = false;
					}
					else
					{
						particlesRaw[i].isBorn = true;
					}
				}

				if (tmpTime < 0)
				{
					particlesRaw[i].isExist = false;
					particlesRaw[i].isBorn = false;
				}
			}
		}

	}

	//================================================================================
	int32 EffectEmitter::getTimeLength() const noexcept
	{
		return emitterParam.life + (emitterParam.particleLife + emitterParam.particleLifeDiff);
	}

}
