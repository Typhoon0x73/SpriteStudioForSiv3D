#pragma once
#include <Siv3D.hpp>
#include "Setting/SSAnimationSetting.hpp"
#include "Model/SSAnimationModel.hpp"
#include "Animation/SSAnimation.hpp"

namespace s3d::SpriteStudio
{
	/// @brief アニメーションパック
	struct AnimationPack
	{
		/// @brief パック名
		String name{ U"" };

		/// @brief アニメーション設定
		AnimationSetting setting{};

		/// @brief アニメーション用モデル
		AnimationModel model{};

		/// @brief 参照するセットアップアニメーション
		Animation* pSetupAnimation{ nullptr };

		/// @brief 利用するセルマップ名一覧
		Array<String> cellmapNames{};

		/// @brief パックに含まれるアニメーションデータ
		Array<Animation> animations{};
	};
}
