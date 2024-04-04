#pragma once
#include <Siv3D.hpp>
#include "../../Common/SSTypeDefine.hpp"
#include "../../Common/SSTreeNode.hpp"
#include "SSEffectBehavior.hpp"

namespace s3d::SpriteStudio
{
	/// @brief エフェクトノード
	struct EffectNode : public TreeNode
	{
		/// @brief 管理配列のインデックス
		int32 arrayIndex{ -1 };

		/// @brief 親のインデックス
		int32 parentIndex{ -1 };

		/// @brief ノードの種類
		EffectNodeType type{ EffectNodeType::Invalid };

		/// @brief 可視フラグ
		bool isVisible{ false };

		/// @brief 振る舞い
		Optional<EffectBehavior> behaviorOpt{ none };
	};
}
