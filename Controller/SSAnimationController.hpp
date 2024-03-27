#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief アニメーション制御
	class AnimationController
	{
	private:

		/// @brief プロジェクトデータの参照
		const Project* m_pProject;

		/// @brief 再生中アニメーションパックのキャッシュ
		const AnimationPack* m_pCurrentAnimationPack;

		/// @brief 再生中アニメーションのキャッシュ
		const Animation* m_pCurrentAnimation;
	};
}
