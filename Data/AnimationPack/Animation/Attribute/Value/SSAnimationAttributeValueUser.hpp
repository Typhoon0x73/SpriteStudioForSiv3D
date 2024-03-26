#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief ユーザー指定データ
	using AttributeValueUserVariant = std::variant<
		std::monostate
		, Float2
		, int32
		, RectF
		, String
	>;

	/// @brief ユーザーパラメータ
	struct AttributeValueUser
	{
		/// @brief 値
		AttributeValueUserVariant param{ std::monostate{} };
	};
}
