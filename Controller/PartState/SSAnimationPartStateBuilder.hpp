#pragma once
#include <Siv3D.hpp>
#include "../../Data/Project/SSProject.hpp"
#include "SSAnimationPartState.hpp"

namespace s3d::SpriteStudio
{
	/// @brief アニメーションパーツの状態を構築するクラス
	class AnimationPartStateBuilder
	{
	public:

		AnimationPartStateBuilder() = delete;
		virtual ~AnimationPartStateBuilder() = delete;

		/// @brief パーツ情報から状態を構築します。
		/// @param pProject プロジェクトデータへの参照
		/// @param pModelPart モデルパーツ
		/// @param pSetupPart セットアップアニメーションパーツ
		/// @param pAnimationPart アニメーションパーツ
		/// @param pCell セル参照
		/// @param pCellmap セルマップ参照
		/// @param pTexture 画像参照
		/// @return 構築が成功すれば、newされたインスタンスが返ります。 それ以外 nullptr
		[[nodiscard]]
		static AnimationPartState* Build(const Project* pProject, const AnimationModelPart* pModelPart, const AnimationPart* pSetupPart, const AnimationPart* pAnimationPart, const CellmapTextureInfo& cellmapTextureInfo);
	};
}
