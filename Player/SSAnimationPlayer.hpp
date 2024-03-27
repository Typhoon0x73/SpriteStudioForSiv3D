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

		[[nodiscard]]
		AnimationPlayer();

		[[nodiscard]]
		explicit AnimationPlayer(const Project* pProject, StringView animationPack, StringView animation);

		virtual ~AnimationPlayer();

		[[nodiscard]]
		bool build(const Project* pProject, StringView animationPack, StringView animation);

		void update(double delta = Scene::DeltaTime());

		void draw(double x, double y) const;

		[[nodiscard]]
		bool changeAnimation(StringView animationPack, StringView animation);

		[[nodiscard]]
		bool changeAnimation(StringView animation);

		void stop();

		void resume();

		void reset();

		void setLoop(LoopEnable enable);

		void setFrame(int32 frame);

		void setTime(SecondsF seconds);

		[[nodiscard]]
		bool isStopped() const noexcept;

		[[nodiscard]]
		bool isEnd() const noexcept;

		[[nodiscard]]
		bool isValid() const noexcept;

	private:

		/// @brief アニメーション制御
		AnimationController m_animationController;
	};
}
