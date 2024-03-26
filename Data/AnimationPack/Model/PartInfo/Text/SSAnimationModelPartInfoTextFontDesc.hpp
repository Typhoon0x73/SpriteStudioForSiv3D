#pragma once
#include <Siv3D.hpp>
#include "../../../../../Common/SSTypeDefine.hpp"

namespace s3d::SpriteStudio
{
	/// @brief テキストパーツのフォント設定
	struct ModelPartInfoTextFontDesc
	{
		/// @brief ビットマップ
		bool isBitmap{ false };

		/// @brief ファミリ
		String family{ U"" };

		/// @brief キャラマップ
		String charMap{ U"" };

		/// @brief サイズ
		int32 size{ 1 };

		/// @brief スペース
		float space{ 0.0f };

		/// @brief アンカー
		Anchor anchor{ Anchor::None };

		/// @brief 更新が必要か
		bool isUpdate{ false };
	};
}
