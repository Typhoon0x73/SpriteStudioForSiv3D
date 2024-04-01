﻿#pragma once
#include <Siv3D.hpp>
#include "../Controller/SSAnimationController.hpp"

namespace s3d::SpriteStudio
{
	/// @brief アニメーション表示クラス
	class AnimationDrawer
	{
	public:

		/// @brief ブレンドステートを取得します。
		/// @param blendType ブレンドの種類
		/// @return ステート
		static BlendState GetBlendState(BlendType blendType);

		/// @brief コンストラクタ
		[[nodiscard]]
		AnimationDrawer();

		/// @brief アニメーションコントローラーを指定して作成します。
		/// @param pController 参照するアニメーションコントローラー
		[[nodiscard]]
		explicit AnimationDrawer(const AnimationController* pController);

		/// @brief デストラクタ
		virtual ~AnimationDrawer();

		/// @brief デフォルトの状態にリセットします。
		void resetDefault();

		/// @brief 参照するアニメーションコントローラーを設定します。
		/// @param pController 参照するアニメーションコントローラー
		void setAnimationController(const AnimationController* pController);

		/// @brief 当たり判定の表示色を設定します。
		/// @param color 塗りつぶしの色
		void setBoundsColor(const ColorF& color);

		/// @brief 当たり判定の表示色を設定します。
		/// @param fillColor 塗りつぶしの色
		/// @param frameColor 枠の色
		void setBoundsColor(const ColorF& fillColor, const ColorF& frameColor);

		/// @brief 当たり判定の表示色を設定します。
		/// @param color 枠の色
		void setBoundsFrameColor(const ColorF& color);

		/// @brief 設定されているアニメーション制御の状態を描画します。
		/// @param pAnimationController 参照するアニメーション制御
		/// @param x 左上X座標
		/// @param y 左上Y座標
		void draw(const AnimationController* pAnimationController, double x, double y) const;

		/// @brief 設定されているアニメーション制御の状態を描画します。
		/// @param x 左上X座標
		/// @param y 左上Y座標
		void draw(double x, double y) const;

		/// @brief 設定されているアニメーション制御の状態を描画します。
		/// @param pos 左上座標
		void draw(const Vec2& pos) const;

		/// @brief 設定されているアニメーション制御の状態を描画します。
		/// @param pAnimationController 参照するアニメーション制御
		/// @param centerX 中央X座標
		/// @param centerY 中央Y座標
		void drawAt(const AnimationController* pAnimationController, double centerX, double centerY) const;

		/// @brief 設定されているアニメーション制御の状態を描画します。
		/// @param centerX 中央X座標
		/// @param centerY 中央Y座標
		void drawAt(double centerX, double centerY) const;

		/// @brief 設定されているアニメーション制御の状態を描画します。
		/// @param center 中央座標
		void drawAt(const Vec2& center) const;

		/// @brief 指定位置をSpriteStudioで設定したピボットの位置に合わせて描画します。
		/// @param pAnimationController 参照するアニメーション制御
		/// @param x 指定X座標
		/// @param y 指定Y座標
		void drawPivot(const AnimationController* pAnimationController, double x, double y) const;

		/// @brief 指定位置をSpriteStudioで設定したピボットの位置に合わせて描画します。
		/// @param x 指定X座標
		/// @param y 指定Y座標
		void drawPivot(double x, double y) const;

		/// @brief 指定位置をSpriteStudioで設定したピボットの位置に合わせて描画します。
		/// @param pos 指定座標
		void drawPivot(const Vec2& pos) const;

	private:

		void drawPartNormal(const AnimationPartState* pPartState) const;
		void drawPartInstance(const AnimationPartState* pPartState, double x, double y) const;
		void drawPartMesh(const AnimationPartState* pPartState) const;

	private:

		const AnimationController* m_pController;
		ColorF m_boundsColor;
		ColorF m_boundsFrameColor;
	};
}
