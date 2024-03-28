#pragma once
#include <Siv3D.hpp>
#include "../Data/Project/SSProject.hpp"
#include "../Controller/SSAnimationController.hpp"

namespace s3d::SpriteStudio
{
	/// @brief ループフラグ指定用
	using LoopEnable = YesNo<struct LoopEnable_tag>;

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

		/// @brief デストラクタ
		virtual ~AnimationPlayer();

		/// @brief プロジェクトとアニメーションを指定してアニメーション制御を作成します。
		/// @param pProject 参照するプロジェクト
		/// @param animationPack 再生するアニメーションを含むアニメーションパック名
		/// @param animation 再生するアニメーション名
		[[nodiscard]]
		bool build(const Project* pProject, StringView animationPack, StringView animation);

		/// @brief 更新
		/// @param delta 更新する時間
		void update(double delta = Scene::DeltaTime());

		/// @brief 描画
		/// @param x 左上X座標
		/// @param y 左上Y座標
		void draw(double x, double y) const;

		/// @brief アニメーションを変更します。
		/// @param animationPack 再生するアニメーションを含むアニメーションパック名
		/// @param animation 再生するアニメーション名
		/// @return 成功で true を返します。それ以外 false
		[[nodiscard]]
		bool changeAnimation(StringView animationPack, StringView animation);

		/// @brief アニメーションを変更します。
		/// @param animation 再生するアニメーション名
		/// @return 成功で true を返します。それ以外 false
		[[nodiscard]]
		bool changeAnimation(StringView animation);

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
		int32 calcFrameByTime() const;

	private:

		/// @brief アニメーション制御
		AnimationController m_animationController;

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
