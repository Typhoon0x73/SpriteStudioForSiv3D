#pragma once
#include <Siv3D.hpp>
#include "../../../../Common/SSTypeDefine.hpp"

namespace s3d::SpriteStudio
{
	/// @brief シェイプパーツの情報
	struct ModelPartInfoShape
	{
		/// @brief シェイプの種類
		ShapeType shapeType{ ShapeType::Invalid };

		/// @brief マスク
		bool isMask{ false };

		/// @brief 更新が必要か
		bool isUpdate{ false };

		/// @brief シェイプ
		Shape2D shape{};
	};
}
