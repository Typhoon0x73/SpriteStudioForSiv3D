#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief インスタンスパラメータ
	struct AttributeValueInstance
	{
		/// @brief 無限ループフラグ
		bool isInfinity{ false };

		/// @brief 逆再生フラグ
		bool isReverse{ false };

		/// @brief 往復再生フラグ
		bool isPingPong{ false };

		/// @brief 独立動作フラグ
		bool isIndependent{ false };

		/// @brief ループ回数 無限ループフラグ=trueの時には無効
		int32 loopNum{ 0 };

		/// @brief 再生開始位置 ラベル名称
		String startLabel{ U"_start" };

		/// @brief 再生開始位置 ラベル名称からのオフセット
		int32 startOffset{ 0 };

		/// @brief 再生終了位置 ラベル名称
		String endLabel{ U"_end" };

		/// @brief 再生終了位置 ラベル名称からのオフセット
		int32 endOffset{ 0 };

		/// @brief 再生スピード
		float speed{ 0.0f };

		/// @brief ラベル位置とオフセット位置を加えた実際のフレーム数
		int32 startFrame{ 0 };

		/// @brief ラベル位置とオフセット位置を加えた実際のフレーム数
		int32 endFrame{ 0 };

		/// @brief この値があるキーフレーム値 (計算値）
		int32 curKeyframe{ 0 };

		/// @brief 再生時間の累積
		float liveFrame{ 0.0f };
	};
}
