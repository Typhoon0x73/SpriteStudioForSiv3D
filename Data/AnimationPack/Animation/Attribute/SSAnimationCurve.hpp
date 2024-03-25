#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief 補間用カーブパラメータ
	struct AnimationCurve
	{
		/// @brief 始点キーの時間から制御点の時間へのオフセット値。X軸に当たる。
		float m_startFrame;

		/// @brief 始点キーの値から制御点の時間へのオフセット値。Y軸に当たる。
		float m_startValue;

		/// @brief 終点キーの時間から制御点の時間へのオフセット値。X軸に当たる。
		float m_endFrame;

		/// @brief 終点キーの値から制御点の時間へのオフセット値。Y軸に当たる。
		float m_endValue;
	};
}
