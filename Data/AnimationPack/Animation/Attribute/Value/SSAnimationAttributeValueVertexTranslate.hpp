#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief 頂点変形パラメータ
	struct AttributeValueVertexTranslate
	{
		/// @brief ４頂点分の移動値
		Float2 trans[4];
	};
}
