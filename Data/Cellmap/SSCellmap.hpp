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
		String name;

		/// @brief 参照画像ファイルパス。プロジェクト設定の画像基準相対
		FilePath imagePath;

		/// @brief 画像のピクセルWHサイズ
		Size pixelSize;

		/// @brief nullptrでなければプロジェクトの設定を使わずこちらを使用する
		std::unique_ptr<ProjectSetting> pOverrideSetting;

		/// @brief セルリスト
		Array<Cell> cells;
	};
}
