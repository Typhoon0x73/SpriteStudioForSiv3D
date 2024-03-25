#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief 画像切り替えパラメータ
	struct AttributeValueTextureChange
	{
		/// @brief 画像ファイル名(拡張子含む)
		String textureName;
	};
}
