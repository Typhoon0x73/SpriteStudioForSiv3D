#pragma once
#include <Siv3D.hpp>
#include "SSCellMeshInfo.hpp"

namespace s3d::SpriteStudio
{
	/// @brief セル
	struct Cell
	{
		/// @brief 名前
		String name;

		/// @brief 切り抜き矩形
		Rect rect;

		/// @brief 原点。(rect.size/2) が中央=(0,0)になる。
		Float2 pivot;

		/// @brief 左方向に９０度回転されている。uvの割り当てが変わる。
		bool isRotated;

		/// @brief  親テクスチャのサイズ
		Size textureSize;

		/// @brief メッシュ情報。nullptrでなければメッシュ
		std::unique_ptr<CellMeshInfo> pMeshInfo;
	};
}
