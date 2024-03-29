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

		/// @brief 最初のキーフレーム値を取得します。
		/// @return データがあれば先頭のキーフレーム値を返します。それ以外 nullptr
		[[nodiscard]]
		const AnimationKeyFrame* const getFirstKey() const noexcept;

		/// @brief 指定フレームから左側(keyframe.frame <= frame)にあるキーフレーム値を探します。
		/// @param frame 指定フレーム
		/// @return 指定フレームの左側のキーフレーム値を返します。見つからなければ nullptr
		[[nodiscard]]
		const AnimationKeyFrame* const findLeftKey(int32 frame) const noexcept;

		/// @brief 指定フレームから右側(frame < keyframe.frame)にあるキーフレーム値を探します。
		/// @param frame 指定フレーム
		/// @return 指定フレームの右側のキーフレーム値を返します。見つからなければ nullptr
		[[nodiscard]]
		const AnimationKeyFrame* const findRightKey(int32 frame) const noexcept;
	};
}
