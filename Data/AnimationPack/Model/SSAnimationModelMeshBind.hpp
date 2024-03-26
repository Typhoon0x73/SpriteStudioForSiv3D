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
			String boneName{ U"" };

			/// @brief 重み
			int32 weight{ 0 };

			/// @brief ボーンインデックス
			int32 boneIndex{ -1 };

			/// @brief オフセット
			Float2 offset{ Float2::Zero() };
		};
		using BindBoneInfomations = std::array<Info, MeshBindBoneMax>;

		/// @brief メッシュバインド名
		String name{ U"" };

		/// @brief 頂点毎のバインド情報
		Array<BindBoneInfomations> vertexBinds{};

		/// @brief 有効なバインド数
		int32 bindBoneNum{ 0 };
	};
}
