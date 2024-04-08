#pragma once
#include <Siv3D.hpp>

namespace s3d::SpriteStudio
{
	/// @brief パーティクルパラメータ
	struct EffectParticleParam
	{
		/// @brief 拡縮
		Float2 scale{ Float2::One() };

		/// @brief スタート時のカラー
		Color startColor{ Palette::White };

		/// @brief 終了時のカラー
		Color endColor{ Palette:: White };

		/// @brief 初速
		float speed{ 0.0f };

		/// @brief 初速(最大値-最小値)
		float speedDiff{ 0.0f };

		/// @brief 向いてる角度(radian)
		float angle{ 0.0f };

		/// @brief 変差(radian)
		float angleVariance{ 0.0f };

		/// @brief 重力
		Optional<Float2> gravityOpt{ none };

		/// @brief オフセット使用
		bool useOffset{ false };

		/// @brief オフセット
		Float2 offset{ Float2::Zero() };

		/// @brief オフセット(最大値-最小値)
		Float2 offsetDiff{ Float2::Zero() };

		/// @brief 回転の使用
		bool useRotation{ false };

		/// @brief 回転
		float rotation{ 0.0f };

		/// @brief 回転(最大値-最小値)
		float rotationDiff{ 0.0f };

		/// @brief 回転加算値
		float rotationAdd{ 0.0f };

		/// @brief 回転加算(最大値-最小値)
		float rotationAddDiff{ 0.0f };

		/// @brief 回転
		bool useRotationTrans{ false };

		/// @brief 
		float rotationFactor{ 0.0f };

		/// @brief 
		float endLifeTimePer{ 0.0f };

		/// @brief 
		bool useTanAccel{ false };

		/// @brief 
		float tangentialAccel{ 0.0f };

		/// @brief 
		float tangentialAccelDiff{ 0.0f };

		/// @brief 
		bool useInitColor{ false };

		/// @brief 
		Color initColor{ Palette::White };

		/// @brief 
		Color initColorDiff{ Palette::White };

		/// @brief 
		bool useTransColor{ false };

		/// @brief 
		Color transColor{ Palette::White };

		/// @brief 
		Color transColorDiff{ Palette::White };

		/// @brief 
		bool useInitScale{ false };

		/// @brief 
		Float2 scaleRange{ Float2::Zero() };

		/// @brief 
		float scaleFactor{ 0.0f };

		/// @brief 
		float scaleFactorDiff{ 0.0f };

		/// @brief 
		bool useTransScale{ false };

		/// @brief 
		Float2 transScale{ Float2::Zero() };

		/// @brief 
		Float2 transScaleRange{ Float2::Zero() };

		/// @brief 
		float transScaleFactor{ 0.0f };

		/// @brief 
		float transScaleFactorDiff{ 0.0f };

		/// @brief 
		float delay{ 0.0f };

		/// @brief 
		bool usePointGravity{ false };

		/// @brief 
		Float2 gravityPos{ Float2::Zero() };

		/// @brief 
		float gravityPower{ 0.0f };

		/// @brief 
		bool useAlphaFade{ false };

		/// @brief 
		float alphaFade{ 0.0f };

		/// @brief 
		float alphaFadeMax{ 0.0f };

		/// @brief 
		bool useTransSpeed{ false };

		/// @brief 
		float transSpeed{ 0.0f };

		/// @brief 
		float transSpeedDiff{ 0.0f };

		/// @brief 
		Optional<float> direcRotAddOpt{ none };

		/// @brief 
		Optional<uint32> overrideRandomSeedOpt{ none };

	};
}
