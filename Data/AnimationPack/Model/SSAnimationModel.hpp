#pragma once
#include <Siv3D.hpp>
#include "SSAnimationModelPart.hpp"
#include "SSAnimationModelMeshBind.hpp"

namespace s3d::SpriteStudio
{
	/// @brief アニメーションモデル
	struct AnimationModel
	{
		/// @brief パーツリスト
		Array<AnimationModelPart> parts;

		/// @brief メッシュバインドリスト
		Array<AnimationModelMeshBind> meshBinds;
	};
}
