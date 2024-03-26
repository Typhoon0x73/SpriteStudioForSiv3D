#pragma once
#include <Siv3D.hpp>
#include "../Setting/SSAnimationSetting.hpp"
#include "Label/SSAnimationLabel.hpp"
#include "SSAnimationPart.hpp"

namespace s3d::SpriteStudio
{
	/// @brief アニメーションデータ
	struct Animation
	{
		/// @brief アニメーション名
		String name{ U"" };

		/// @brief アニメーションの設定。noneの場合、packのアニメーション設定を利用する。
		Optional<AnimationSetting> overrideSetting{ none };

		/// @brief ラベル
		Array<Label> labels{};

		/// @brief アニメーションする各パーツ
		Array<AnimationPart> animationParts{};

		/// @brief セットアップアニメーションか
		bool isSetup{ false };
	};
}
