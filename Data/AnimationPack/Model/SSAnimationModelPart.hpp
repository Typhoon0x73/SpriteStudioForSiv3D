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
		String name;

		/// @brief ツリーを配列にしたときのインデックス
		int32 arrayIndex;

		/// @brief 親パーツのインデックス
		int32 parentIndex;

		/// @brief パーツの種類
		PartType partType;

		/// @brief 当たり判定形状の種類
		BoundsType boundsType;

		/// @brief 継承の種類
		InheritType inheritType;

		/// @brief アルファブレンドの種類
		BlendType blendType;

		/// @brief マスクの影響を受けるかどうか
		bool isMaskInfluence;

		/// @brief 親のアルファ値の継承率
		float alphaIheritRate;

		/// @brief パーツ毎の情報
		ModelPartInfoVariant partVariantValue;
	};
}
