#pragma once
#include <Siv3D.hpp>
#include "../Project/Setting/SSProjectSetting.hpp"
#include "Cell/SSCell.hpp"

namespace s3d::SpriteStudio
{
	/// @brief セルマップ
	struct Cellmap
	{
		/// @brief セルマップ名
		String name{ U"" };

		/// @brief 参照画像ファイルパス。プロジェクト設定の画像基準相対
		FilePath imagePath{ U"" };

		/// @brief 画像のピクセルWHサイズ
		Size pixelSize{ Size::One() };

		/// @brief noneでなければプロジェクトの設定を使わずこちらを使用する
		Optional<ProjectSetting> overrideSettingOpt{ none };

		/// @brief セルリスト
		Array<Cell> cells{};
	};
}
