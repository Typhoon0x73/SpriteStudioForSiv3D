﻿#pragma once
#include <Siv3D.hpp>
#include "../Data/Project/SSProject.hpp"
#include "../Controller/SSAnimationController.hpp"
#include "../Drawer/SSAnimationDrawer.hpp"

namespace s3d::SpriteStudio
{
	/// @brief ループフラグ指定用
	using LoopEnable = YesNo<struct LoopEnable_tag>;

	/// @brief 同じアニメーションを指定していても切り替えるか
	using SameChange = YesNo<struct SameChange_tag>;

	/// @brief changeAnimation関数で利用する動作設定。
	enum class ChangeAnimationSetting
	{
		None,     // 無いものはない。切り替えできなければ false を返す。
		AddBuild, // 配列に無い場合は、その場でbuildして問題が無ければ配列へ追加し切り替える。
	};

	/// @brief ビルド用情報
	struct AnimationBuildInfo
	{
		/// @brief パック単位
		struct PackInfo
		{
			/// @brief アニメーションパック名
			String animationPackName{ U"" };

			/// @brief アニメーション名配列
			Array<String> animationNames{};
		};

		/// @brief アニメーションパックを含むプロジェクト参照
		const Project* pProject{ nullptr };

		/// @brief パック単位情報配列
		Array<PackInfo> packInfomations{};
	};

	/// @brief アニメーション再生
	class AnimationPlayer
	{
	public:

		/// @brief コンストラクタ
		[[nodiscard]]
		AnimationPlayer();

		/// @brief プロジェクトとアニメーションを指定してアニメーション制御を作成します。
		/// @param pProject 参照するプロジェクト
		/// @param animationPack 再生するアニメーションを含むアニメーションパック名
		/// @param animation 再生するアニメーション名
		[[nodiscard]]
		explicit AnimationPlayer(const Project* pProject, StringView animationPack, StringView animation);

		/// @brief プロジェクトとアニメーションを指定してアニメーション制御を作成します。
		/// @param buildInfo まとめて生成する用のビルド情報
		[[nodiscard]]
		explicit AnimationPlayer(const AnimationBuildInfo& buildInfo);

		/// @brief デストラクタ
		virtual ~AnimationPlayer();

		/// @brief プロジェクトとアニメーションを指定してアニメーション制御を作成します。
		/// @param pProject 参照するプロジェクト
		/// @param animationPack 再生するアニメーションを含むアニメーションパック名
		/// @param animation 再生するアニメーション名
		/// @return 成功で true を返します。それ以外 false
		[[nodiscard]]
		bool build(const Project* pProject, StringView animationPack, StringView animation);

		/// @brief プロジェクトとアニメーションを指定してアニメーション制御を作成します。
		/// @param buildInfo まとめて生成する用のビルド情報
		/// @return 成功で true を返します。それ以外 false
		[[nodiscard]]
		bool build(const AnimationBuildInfo& buildInfo);

		/// @brief 更新
		/// @param delta 更新する時間
		void update(double delta = Scene::DeltaTime());

		/// @brief 描画
		/// @param x 左上X座標
		/// @param y 左上Y座標
		void draw(double x, double y) const;

		/// @brief 描画
		/// @param pos 左上座標
		void draw(const Vec2& pos) const;

		/// @brief 描画
		/// @param centerX 中央X座標
		/// @param centerY 中央Y座標
		void drawAt(double centerX, double centerY) const;

		/// @brief 描画
		/// @param center 中央座標
		void drawAt(const Vec2& center) const;

		/// @brief 指定位置をSpriteStudioで設定したピボットの位置に合わせて描画します。
		/// @param x 指定X座標
		/// @param y 指定Y座標
		void drawPivot(double x, double y) const;

		/// @brief 指定位置をSpriteStudioで設定したピボットの位置に合わせて描画します。
		/// @param pos 指定座標
		void drawPivot(const Vec2& pos) const;

		/// @brief アニメーションを変更します。
		/// @param animationPack 再生するアニメーションを含むアニメーションパック名
		/// @param animation 再生するアニメーション名
		/// @param sameChange 同じアニメーションを指定しても切り替え処理をするか
		/// @param changeAnimationSetting 切り替え失敗時の動作
		/// @return 成功で true を返します。それ以外 false
		[[nodiscard]]
		bool changeAnimation(StringView animationPack, StringView animation, SameChange sameChange = SameChange::No, ChangeAnimationSetting changeAnimationSetting = ChangeAnimationSetting::None);

		/// @brief アニメーションを変更します。
		/// @param animation 再生するアニメーション名
		/// @param sameChange 同じアニメーションを指定しても切り替え処理をするか
		/// @param changeAnimationSetting 切り替え失敗時の動作
		/// @return 成功で true を返します。それ以外 false
		[[nodiscard]]
		bool changeAnimation(StringView animation, SameChange sameChange = SameChange::No, ChangeAnimationSetting changeAnimationSetting = ChangeAnimationSetting::None);

		/// @brief アニメーションを変更します。
		/// @param index 再生するアニメーションインデックス
		/// @param sameChange 同じアニメーションを指定しても切り替え処理をするか
		/// @return 成功で true を返します。それ以外 false
		[[nodiscard]]
		bool changeAnimation(int32 index, SameChange sameChange = SameChange::No);

		/// @brief 全ての情報を破棄し、初期化します。
		void clear();

		/// @brief 一時停止します。
		void stop();

		/// @brief 一時停止状態から再開します。
		void resume();

		/// @brief 再生フレームを開始フレームにします。
		void resetFrame();

		/// @brief ループを設定します。
		/// @param enable Yesでループする。 Noでループしない
		void setLoop(LoopEnable enable);

		/// @brief アニメーションの再生時間をフレームで指定します。
		/// @param frame 指定するフレーム
		void setFrame(int32 frame);

		/// @brief アニメーションの再生時間を指定します。
		/// @param seconds 指定する時間（秒）
		void setTime(SecondsF seconds);

		/// @brief 現在再生中のアニメーションのFPSを取得します。
		/// @return アニメーション設定が無ければ 0 が返ります。
		[[nodiscard]]
		int32 getCurrentAnimationFPS() const noexcept;

		/// @brief アニメーションが止まっているかを返します。
		/// @return isEnd() が true もしくは stop() により停止していれば true を返します。 それ以外 false
		[[nodiscard]]
		bool isStopped() const noexcept;

		/// @brief アニメーションが終了しているかを返します。
		/// @remarks isLoop()がtrueを返す場合、trueになることはありません。
		/// @return 最終フレームの更新を終えたら true を返します。 それ以外 false
		[[nodiscard]]
		bool isEnd() const noexcept;

		/// @brief ループするかを返します。
		/// @return true でループします。
		[[nodiscard]]
		bool isLoop() const noexcept;

		/// @brief アニメーション再生できる状態かを返します。
		/// @return 再生可能であれば true を返します。 それ以外 false
		[[nodiscard]]
		bool isReady() const noexcept;

	private:

		/// @brief 時間からフレームを算出します。
		/// @return フレーム
		[[nodiscard]]
		int32 calcFrameByTime() const noexcept;

	private:

		/// @brief プロジェクト参照
		const Project* m_pProject;

		/// @brief アニメーション制御
		Array<std::unique_ptr<AnimationController>> m_animationControllers;

		/// @brief 現在再生中のアニメーション制御番号
		int32 m_currentAnimationIndex;

		/// @brief 描画
		AnimationDrawer m_drawer;

		/// @brief フレーム
		int32 m_frame;

		/// @brief 再生時間
		double m_time;

		/// @brief 停止フラグ
		bool m_isStop;

		/// @brief ループフラグ
		bool m_isLoop;
	};
}
