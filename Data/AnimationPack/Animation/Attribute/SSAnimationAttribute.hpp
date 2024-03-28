#pragma once
#include <Siv3D.hpp>
#include "../../../../Common/SSTypeDefine.hpp"
#include "SSAnimationKeyFrame.hpp"

namespace s3d::SpriteStudio
{
	/// @brief アニメーションアトリビュート
	struct AnimationAttribute
	{
		/// @brief アトリビュートの種類
		AttributeKind kind{ AttributeKind::Invalid };

		/// @brief アトリビュートのキーフレーム
		Array<AnimationKeyFrame> keyFrames{};

		/// @brief 最初のキーデータを取得します。
		/// @return データがあれば先頭のキーを返します。それ以外 nullptr
		[[nodiscard]]
		const AnimationKeyFrame* const getFirstKey() const noexcept;
	};
}
