#pragma once
#include <Siv3D.hpp>
#include "../../../Data/AnimationPack/Animation/Attribute/Value/SSAnimationAttributeValueCell.hpp"
#include "../../../Common/SSXORShift32.hpp"
#include "SSEffectEmitterParam.hpp"
#include "Pattern/SSEffectEmitterPattern.hpp"
#include "../Particle/SSEffectParticle.hpp"
#include "../Particle/SSEffectParticleDrawData.hpp"
#include "../Particle/SSEffectParticleParam.hpp"
#include "../../../Data/Cellmap/Cell/SSCell.hpp"
#include "../../../Data/Effect/SSEffectBehavior.hpp"

namespace s3d::SpriteStudio
{
	/// @brief エミッタ
	struct EffectEmitter
	{
		/// @brief 親配列のインデックス
		int32 parentIndex{ -1 };

		/// @brief 参照セル情報
		AttributeValueCell cell{};

		/// @brief 優先度
		int32 priority{ 0 };

		/// @brief エミッタのパラメータ
		EffectEmitterParam emitterParam{};

		/// @brief パーティクルパラメータ
		EffectParticleParam particleParam{};

		/// @brief 乱数値
		XORShift32 random{};

		/// @brief エミッタのシード値
		uint32 emitterSeed{ 0 };

		/// @brief シードのオフセット
		int32 seedOffset{ 0 };

		/// @brief 
		Array<EffectEmitterPattern> emitPatterns{};

		/// @brief 
		Array<int32> offsetPatterns{};

		/// @brief 
		Array<EffectParticle> particles{};

		/// @brief 
		Array<EffectParticleDrawData> particleDrawInfos{};

		/// @brief 
		int32 particleIdMax{ 0 };

		/// @brief 
		size_t particleListBufferSize{ 0 };

		/// @brief 
		Array<uint32> seeds{};

		/// @brief 
		Float2 position{ Float2::Zero() };

		/// @brief 
		EffectEmitter* pParent{ nullptr };

		/// @brief 
		const Cell* pCell{ nullptr };

		/// @brief 
		const Texture* pTexture{ nullptr };

		/// @brief 
		const EffectBehavior* pBehavior{ nullptr };

		/// @brief 
		size_t globalTime{ 0 };

		/// @brief 
		size_t seedTableLength{ 0 };

		/// @brief 
		uint32 uniqueId{ 0 };

		/// @brief 事前計算
		void preCalculate();

		/// @brief エミッタを更新します。
		/// @param targetFrame 
		/// @param offset 
		void update(int32 targetFrame, int32 offset);

		void updateParticle(int32 frame, EffectParticleDrawData* pParticleData);

		/// @brief 
		/// @return 
		[[nodiscard]]
		int32 getTimeLength() const noexcept;
	};
}
