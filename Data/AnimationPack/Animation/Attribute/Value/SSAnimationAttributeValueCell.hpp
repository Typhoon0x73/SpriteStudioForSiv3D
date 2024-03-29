#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief 参照セルパラメータ
	struct AttributeValueCell
	{
		/// @brief AnimationPackのCellmapNamesに対応したインデックス
		int32 mapId{ -1 };

		/// @brief Cellmap内の参照するCell名
		String name{ U"" };

		[[nodiscard]]
		inline bool operator ==(const AttributeValueCell& right) noexcept;

		[[nodiscard]]
		inline bool operator !=(const AttributeValueCell& right) noexcept;
	};

#include "SSAnimationAttributeValueCell.ipp"
}
