#pragma once
#include <Siv3D.hpp>
#include "../../../Common/SSTypeDefine.hpp"
#include "PartInfo/SSAnimationModelPartInfo.hpp"

namespace s3d::SpriteStudio
{
	/// @brief アニメーションモデルパーツ
	struct AnimationModelPart
	{
		/// @brief 名前
		String name{ U"" };

		/// @brief ツリーを配列にしたときのインデックス
		int32 arrayIndex{ -1 };

		/// @brief 親パーツのインデックス
		int32 parentIndex{ -1 };

		/// @brief パーツの種類
		PartType partType{ PartType::Invalid };

		/// @brief 当たり判定形状の種類
		BoundsType boundsType{ BoundsType::Invalid };

		/// @brief 継承の種類
		InheritType inheritType{ InheritType::Invalid };

		/// @brief アルファブレンドの種類
		BlendType blendType{ BlendType::Invalid };

		/// @brief マスクの影響を受けるかどうか
		bool isMaskInfluence{ false };

		/// @brief 親のアルファ値の継承率
		float alphaIheritRate{ 1.0f };

		/// @brief パーツ毎の情報
		ModelPartInfoVariant partVariantValue{ std::monostate{} };
	};
}
