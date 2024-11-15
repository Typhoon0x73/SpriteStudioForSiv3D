#pragma once
#include <Siv3D.hpp>
#include "../Cellmap/Cell/SSCell.hpp"
#include "../../Common/SSTypeDefine.hpp"
#include "Element/SSEffectElement.hpp"

namespace s3d::SpriteStudio
{
	/// @brief エフェクトの振る舞い
	struct EffectBehavior
	{
		/// @brief 効果一覧
		Array<EffectElementVariant> elements;

		/// @brief セル名
		String cellName;

		/// @brief セルマップ名
		String cellmapName;

		/// @brief ブレンドの種類
		BlendType blendType;
	};
}
