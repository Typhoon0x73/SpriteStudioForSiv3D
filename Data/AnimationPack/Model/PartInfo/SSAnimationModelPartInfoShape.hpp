#pragma once
#include <Siv3D.hpp>
#include "../../../../Common/SSTypeDefine.hpp"

namespace s3d::SpriteStudio
{
	/// @brief シェイプパーツの情報
	struct ModelPartInfoShape
	{
		/// @brief シェイプの種類
		ShapeType shapeType;

		/// @brief マスク
		bool isMask;

		/// @brief 更新が必要か
		bool isUpdate;

		/// @brief シェイプ
		Shape2D shape;
	};
}
