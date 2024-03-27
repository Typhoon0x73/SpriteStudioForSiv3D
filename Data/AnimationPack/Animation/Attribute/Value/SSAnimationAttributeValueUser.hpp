#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief ユーザーパラメータ
	struct AttributeValueUser
	{
		/// @brief 整数
		Optional<int32> integerOpt{ none };

		/// @brief 矩形
		Optional<RectF> rectOpt{ none };

		/// @brief 座標
		Optional<Float2> pointOpt{ none };

		/// @brief 文字列
		Optional<String> stringOpt{ none };

	};
}
