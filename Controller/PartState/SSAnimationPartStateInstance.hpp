#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	//================================================================================
	// 前方宣言
	//================================================================================
	class AnimationController;

	//================================================================================
	/// @brief インスタンスパーツ用パラメータ
	struct AnimationPartStateInstance
	{
		/// @brief 参照アニメーション制御用
		std::unique_ptr<AnimationController> pRefAnimationController{ nullptr };
	};
}
