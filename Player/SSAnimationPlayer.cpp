#include "SSAnimationPlayer.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	AnimationPlayer::AnimationPlayer()
		: m_pProject{ nullptr }
		, m_animationControllers{}
		, m_currentAnimationIndex{ -1 }
		, m_drawer{}
		, m_frame{ 0 }
		, m_time{ 0.0 }
		, m_isStop{ true }
		, m_isLoop{ false }
	{
	}

	//================================================================================
	AnimationPlayer::AnimationPlayer(const Project* pProject, StringView animationPack, StringView animation)
		: AnimationPlayer{}
	{
		if (not(build(pProject, animationPack, animation)))
		{
			clear();
		}
	}

	//================================================================================
	AnimationPlayer::AnimationPlayer(const AnimationBuildInfo& buildInfo)
		: AnimationPlayer{}
	{
		if (not(build(buildInfo)))
		{
			clear();
		}
	}

	//================================================================================
	AnimationPlayer::~AnimationPlayer()
	{
	}

	//================================================================================
	bool AnimationPlayer::build(const Project* pProject, StringView animationPack, StringView animation)
	{
		auto pAnimationController = std::make_unique<AnimationController>();
		if (not(pAnimationController->build(pProject, animationPack, animation)))
		{
			return false;
		}
		m_animationControllers.emplace_back(std::move(pAnimationController));
		m_pProject = pProject;
		if (m_currentAnimationIndex < 0)
		{
			m_currentAnimationIndex = 0;
		}
		return true;
	}

	//================================================================================
	bool AnimationPlayer::build(const AnimationBuildInfo& buildInfo)
	{
		for (const auto& packInfo : buildInfo.packInfomations)
		{
			for (const auto& animName : packInfo.animationNames)
			{
				if (not(build(buildInfo.pProject, packInfo.animationPackName, animName)))
				{
					return false;
				}
			}
		}
		return true;
	}

	//================================================================================
	void AnimationPlayer::update(double delta)
	{
		if (m_currentAnimationIndex < 0)
		{
			return;
		}
		m_time += delta;
		m_frame = calcFrameByTime();
	}

	//================================================================================
	void AnimationPlayer::draw(double x, double y) const
	{
	}

	//================================================================================
	void AnimationPlayer::draw(const Vec2& pos) const
	{
	}

	//================================================================================
	void AnimationPlayer::drawAt(double centerX, double centerY) const
	{
	}

	//================================================================================
	void AnimationPlayer::drawAt(const Vec2& center) const
	{
	}

	//================================================================================
	void AnimationPlayer::drawPivot(double x, double y) const
	{
	}

	//================================================================================
	void AnimationPlayer::drawPivot(const Vec2& pos) const
	{
	}

	//================================================================================
	bool AnimationPlayer::changeAnimation(StringView animationPack, StringView animation, SameChange sameChange, ChangeAnimationSetting changeAnimationSetting)
	{
		return false;
	}

	//================================================================================
	bool AnimationPlayer::changeAnimation(StringView animation, SameChange sameChange, ChangeAnimationSetting changeAnimationSetting)
	{
		return false;
	}

	//================================================================================
	bool AnimationPlayer::changeAnimation(int32 index, SameChange sameChange)
	{
		return false;
	}

	//================================================================================
	void AnimationPlayer::clear()
	{
		m_currentAnimationIndex = -1;
		m_drawer.resetDefault();
		m_animationControllers.clear();
		m_pProject = nullptr;
		m_isLoop = false;
		m_isStop = true;
		m_frame = 0;
		m_time = 0.0;
	}

	//================================================================================
	void AnimationPlayer::stop()
	{
		m_isStop = true;
	}

	//================================================================================
	void AnimationPlayer::resume()
	{
		m_isStop = false;
	}

	//================================================================================
	void AnimationPlayer::resetFrame()
	{
		if (not(isReady()))
		{
			return;
		}
		const auto* pAnimationSetting = m_animationControllers[m_currentAnimationIndex]->getAnimationSetting();
		if (pAnimationSetting == nullptr)
		{
			return;
		}
		setFrame(pAnimationSetting->startFrame);
	}

	//================================================================================
	void AnimationPlayer::setLoop(LoopEnable enable)
	{
		m_isLoop = enable.getBool();
	}

	//================================================================================
	void AnimationPlayer::setFrame(int32 frame)
	{
		const int32 fps = getCurrentAnimationFPS();
		if (fps <= 0)
		{
			return;
		}
		m_frame = frame;
		m_time = static_cast<double>(frame) / fps;
	}

	//================================================================================
	void AnimationPlayer::setTime(SecondsF seconds)
	{
		m_time = seconds.count();
		m_frame = calcFrameByTime();
	}

	//================================================================================
	int32 AnimationPlayer::getCurrentAnimationFPS() const noexcept
	{
		if (not(isReady()))
		{
			return 0;
		}
		return m_animationControllers[m_currentAnimationIndex]->getFPS();
	}

	//================================================================================
	bool AnimationPlayer::isStopped() const noexcept
	{
		return m_isStop;
	}

	//================================================================================
	bool AnimationPlayer::isEnd() const noexcept
	{
		if (m_isLoop)
		{
			return false;
		}
		if (not(isReady()))
		{
			return false;
		}
		const auto* pAnimationSetting = m_animationControllers[m_currentAnimationIndex]->getAnimationSetting();
		if (pAnimationSetting == nullptr)
		{
			return true;
		}
		return (pAnimationSetting->endFrame < m_frame);
	}

	//================================================================================
	bool AnimationPlayer::isLoop() const noexcept
	{
		return m_isLoop;
	}

	//================================================================================
	bool AnimationPlayer::isReady() const noexcept
	{
		return (0 <= m_currentAnimationIndex and m_currentAnimationIndex < m_animationControllers.size());
	}

	//================================================================================
	int32 AnimationPlayer::calcFrameByTime() const noexcept
	{
		return static_cast<int32>(m_time * getCurrentAnimationFPS());
	}

}
