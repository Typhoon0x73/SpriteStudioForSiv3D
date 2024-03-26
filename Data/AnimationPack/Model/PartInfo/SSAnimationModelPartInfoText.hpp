#pragma once
#include <Siv3D.hpp>
#include "Text/SSAnimationModelPartInfoTextFontDesc.hpp"
namespace s3d::SpriteStudio
{
	/// @brief テキストパーツの情報
	struct ModelPartInfoText
	{
		/// @brief テキスト
		String text{ U"" };

		/// @brief フォント情報
		ModelPartInfoTextFontDesc fontDesc{};

		/// @brief スムース
		bool isSmooth{ false };

		/// @brief マスク
		bool isMask{ false };

		/// @brief 幅
		int32 width{ 1 };

		/// @brief 高さ
		int32 height{ 1 };

		/// @brief 更新が必要か
		bool isUpdate{ false };
	};
}
