#pragma once
#include <Siv3D.hpp>
#include "../../../../../Common/SSTypeDefine.hpp"

namespace s3d::SpriteStudio
{
	/// @brief テキストパーツのフォント設定
	struct ModelPartInfoTextFontDesc
	{
		/// @brief ビットマップ
		bool isBitmap;

		/// @brief ファミリ
		String family;

		/// @brief キャラマップ
		String charMap;

		/// @brief サイズ
		int32 size;

		/// @brief スペース
		float space;

		/// @brief アンカー
		Anchor anchor;

		/// @brief 更新が必要か
		bool isUpdate;
	};
}
