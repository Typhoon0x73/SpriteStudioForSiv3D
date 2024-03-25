#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief デフォームパラメータ
	struct AttributeValueDeform
	{
		/// @brief 頂点変形リスト
		Array<Float2> verticesChanges;
	};
}
