#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief セルのメッシュ情報
	struct CellMeshInfo
	{
		/// @brief 座標リスト
		Array<Float2> points;

		/// @brief トライアングルリスト
		Array<TriangleIndex> triangles;
	};
}
