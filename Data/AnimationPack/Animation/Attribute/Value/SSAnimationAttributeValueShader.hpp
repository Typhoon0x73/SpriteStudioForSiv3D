#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief シェーダー利用時のパラメータ
	struct AttributeValueShader
	{
		/// @brief シェーダー識別ID
		String id{ U"" };

		/// @brief 各パラメータ
		float param[4]{ 0.0f, 0.0f, 0.0f, 0.0f };
	};
}
