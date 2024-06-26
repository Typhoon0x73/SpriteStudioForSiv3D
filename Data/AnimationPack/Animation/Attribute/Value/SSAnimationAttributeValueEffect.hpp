﻿#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief エフェクトパラメータ
	struct AttributeValueEffect
	{
		/// @brief 開始フレーム
		int32 startTime{ 0 };

		/// @brief 再生速度
		float speed{ 0.0f };

		/// @brief 独立動作
		bool isIndependent{ false };

		/// @brief アトリビュートの初期化フラグ
		bool isAttributeInitialized{ false };

		/// @brief キーが配置されたフレーム 
		int32 currentKeyFrame{ 0 };
	};
}
