#pragma once
#include <Siv3D.hpp>
#include "../../Common/SSTypeDefine.hpp"
#include "../../Data/Cellmap/Cell/SSCell.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	// 前方宣言
	//================================================================================
	struct AnimationPartState;

	//================================================================================
	/// @brief メッシュパーツ用パラメータ
	struct AnimationPartStateMesh
	{
		/// @brief ボーンウェイト
		struct BoneWeight
		{
			/// @brief 情報
			struct Info
			{
				/// @brief ウェイト
				int32 weight{ 0 };

				/// @brief 長さ
				int32 length{ 0 };

				/// @brief オフセット
				Float2 offset{ Float2::Zero() };

				/// @brief アニメーションの状態
				const AnimationPartState* pState{ nullptr };
			};

			/// @brief ウェイト情報
			std::array<Info, MeshBindBoneMax> infomations;

			/// @brief 有効なデータ数
			int32 bindBoneCount;
		};

		/// @brief 描画用バッファ2D
		Buffer2D buffer2D{};

		/// @brief 描画用参照画像
		const Texture* pTexture{ nullptr };

		/// @brief セル参照
		const Cell* pCell{ nullptr };

		/// @brief 計算用
		Array<Float2> vertexPositions{};

		/// @brief 計算用
		Array<Float2> worldVertexPositions{};

		/// @brief ボーンウェイトリスト
		Array<BoneWeight> bindBoneInfomations{};
	};
}
