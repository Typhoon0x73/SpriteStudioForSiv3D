#pragma once
#include <Siv3D.hpp>
#include "SSCellMeshInfo.hpp"

namespace s3d::SpriteStudio
{
	/// @brief セル
	struct Cell
	{
		/// @brief 名前
		String name{ U"" };

		/// @brief 切り抜き矩形
		Rect rect{ Rect::Empty() };

		/// @brief 原点。(rect.size/2) が中央=(0,0)になる。
		Float2 pivot{ Float2::Zero() };

		/// @brief 左方向に９０度回転されている。uvの割り当てが変わる。
		bool isRotated{ false };

		/// @brief  親テクスチャのサイズ
		Size textureSize{ Size::One() };

		/// @brief メッシュ情報。noneでなければメッシュ
		Optional<CellMeshInfo> pMeshInfo{ none };
	};
}
