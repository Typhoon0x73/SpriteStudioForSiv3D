#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief インスタンスパラメータ
	struct AttributeValueInstance
	{
		/// @brief 無限ループフラグ
		bool isInfinity;

		/// @brief 逆再生フラグ
		bool isReverse;

		/// @brief 往復再生フラグ
		bool isPingPong;

		/// @brief 独立動作フラグ
		bool isIndependent;

		/// @brief ループ回数 無限ループフラグ=trueの時には無効
		int32 loopNum;

		/// @brief 再生開始位置 ラベル名称
		String startLabel;

		/// @brief 再生開始位置 ラベル名称からのオフセット
		int32 startOffset;

		/// @brief 再生終了位置 ラベル名称
		String endLabel;

		/// @brief 再生終了位置 ラベル名称からのオフセット
		int32 endOffset;

		/// @brief 再生スピード
		float speed;

		/// @brief ラベル位置とオフセット位置を加えた実際のフレーム数
		int32 startFrame;

		/// @brief ラベル位置とオフセット位置を加えた実際のフレーム数
		int32 endFrame;

		/// @brief この値があるキーフレーム値 (計算値）
		int32 curKeyframe;

		/// @brief 再生時間の累積
		float liveFrame;
	};
}
