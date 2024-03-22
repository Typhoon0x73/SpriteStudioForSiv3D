#pragma once
#include <Siv3D.hpp>
#include "../../../Common/SSTypeDefine.hpp"

namespace s3d::SpriteStudio
{
	/// @brief メッシュバインド
	struct AnimationModelMeshBind
	{
		/// @brief バインド情報
		struct Info
		{
			/// @brief ボーン名
			String boneName;

			/// @brief 重み
			int32 weight;

			/// @brief ボーンインデックス
			int32 boneIndex;

			/// @brief オフセット
			Float2 offset;
		};
		using BindBoneInfomations = std::array<Info, MeshBindBoneMax>;

		/// @brief メッシュバインド名
		String name;

		/// @brief 頂点毎のバインド情報
		Array<BindBoneInfomations> vertexBinds;

		/// @brief 有効なバインド数
		int32 bindBoneNum;
	};
}
