#pragma once
#include <Siv3D.hpp>
#include "Text/SSAnimationModelPartInfoTextFontDesc.hpp"
namespace s3d::SpriteStudio
{
	/// @brief テキストパーツの情報
	struct ModelPartInfoText
	{
		/// @brief テキスト
		String text;

		/// @brief フォント情報
		ModelPartInfoTextFontDesc fontDesc;

		/// @brief スムース
		bool isSmooth;

		/// @brief マスク
		bool isMask;

		/// @brief 幅
		int32 width;

		/// @brief 高さ
		int32 height;

		/// @brief 更新が必要か
		bool isUpdate;
	};
}
