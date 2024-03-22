#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief プロジェクトの設定
	struct ProjectSetting
	{
		/// @brief テクスチャアドレスモード
		TextureAddressMode addressMode;

		/// @brief テクスチャフィルタ
		TextureFilter filterMode;
	};
}
