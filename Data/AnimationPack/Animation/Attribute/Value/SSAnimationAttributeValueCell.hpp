#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief 参照セルパラメータ
	struct AttributeValueCell
	{
		/// @brief AnimationPackのCellmapNamesに対応したインデックス
		int32 mapId;

		/// @brief Cellmap内の参照するCell名
		String name;
	};
}
