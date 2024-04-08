#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief エミッタのパラメータ
	struct EffectEmitterParam
	{
		/// @brief 寿命
		int32 life{ 0 };

		/// @brief 間隔
		int32 interval{ 0 };

		/// @brief 無限生成されるか
		bool isInfinite{ false };

		/// @brief 一度に排出される数
		int32 emitNum{ 0 };

		/// @brief 最大排出量
		int32 emitMax{ 0 };

		/// @brief パーティクルの寿命
		int32 particleLife{ 0 };

		/// @brief パーティクルの寿命(最大値-最小値)
		int32 particleLifeDiff{ 0 };

		/// @brief 
		int32 loopStart{ 0 };

		/// @brief 
		int32 loopEnd{ 0 };

		/// @brief 
		int32 loopLength{ 0 };

		/// @brief 
		int32 loopGenerate{ 0 };

	};
}
