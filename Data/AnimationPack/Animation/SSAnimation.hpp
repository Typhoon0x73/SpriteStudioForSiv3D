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
		String name;

		/// @brief アニメーションの設定。nullptrの場合、packのアニメーション設定を利用する。
		std::unique_ptr<AnimationSetting> pSetting;

		/// @brief ラベル
		Array<Label> labels;

		/// @brief アニメーションする各パーツ
		Array<AnimationPart> animationParts;

		/// @brief セットアップアニメーションか
		bool isSetup;
	};
}
