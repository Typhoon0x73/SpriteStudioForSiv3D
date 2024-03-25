#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief エフェクトパラメータ
	struct AttributeValueEffect
	{
		/// @brief 開始フレーム
		int32 startTime;

		/// @brief 再生速度
		float speed;

		/// @brief 独立動作
		bool isIndependent;

		/// @brief アトリビュートの初期化フラグ
		bool isAttributeInitialized;

		/// @brief キーが配置されたフレーム 
		int32 currentKeyFrame;
	};
}
