#pragma once
#include <Siv3D.hpp>
#include "../../Data/Cellmap/Cell/SSCell.hpp"

namespace s3d::SpriteStudio
{
	/// @brief ノーマルパーツ用パラメータ
	struct AnimationPartStateNormal
	{
		/// @brief 描画用バッファ2D
		Buffer2D buffer2D{ 4U, 2U };
	};
}
