#pragma once
#include <Siv3D.hpp>
#include "../../../../../Common/SSTypeDefine.hpp"

namespace s3d::SpriteStudio
{
	/// @brief パーツカラーパラメータ
	struct AttributeValuePartsColor
	{
		/// @brief ブレンド用カラー値
		struct BlendColorValue
		{
			/// @brief カラー値
			Color rgba{ Palette::White };

			/// @brief 反映率
			float rate{ 1.0f };
		};

		/// @brief ブレンドの適用方法 単色(全体), 頂点単位
		ColorBlendTarget target{ ColorBlendTarget::Invalid };

		/// @brief ブレンドの種類
		BlendType blendType{ BlendType::Invalid };

		/// @brief 頂点単位の場合使用されるカラー値 or target=wholeで単色colors[0]を利用。
		BlendColorValue colors[4]{};
	};
}
