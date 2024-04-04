#pragma once
#include <Siv3D.hpp>
#include "SSEffectNode.hpp"

namespace s3d::SpriteStudio
{
	/// @brief エフェクトモデル
	struct EffectModel
	{
		/// @brief ノード配列
		Array<EffectNode> nodes{};

		/// @brief 乱数固定シード値
		Optional<int32> lockRandomSeedOpt{ none };

		/// @brief FPS
		int32 fps{ 60 };

		/// @brief 名前
		String name{ U"" };

		/// @brief 
		int32 layoutScaleX{ 0 };

		/// @brief 
		int32 layoutScaleY{ 0 };

		/// @brief ルートノードを取得します。
		/// @return ノードが一つもなければ nullptr を返します。
		[[nodiscard]]
		const EffectNode* const getRootNode() const noexcept;
	};
}
