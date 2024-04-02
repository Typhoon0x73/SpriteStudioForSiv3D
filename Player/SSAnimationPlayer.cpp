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
	AnimationPlayer::AnimationPlayer(const Project* pProject, StringView animationPackName, StringView animation, StartPlaying startPlaying)
		: AnimationPlayer{}
	{
		if (not(addBuild(pProject, animationPackName, animation, startPlaying)))
		{
			clear();
		}
	}

	//================================================================================
	AnimationPlayer::AnimationPlayer(const AnimationBuildInfo& buildInfo, StartPlaying startPlaying)
		: AnimationPlayer{}
	{
		if (not(addBuild(buildInfo, startPlaying)))
		{
			clear();
		}
	}

	//================================================================================
	AnimationPlayer::~AnimationPlayer()
	{
	}

	//================================================================================
	bool AnimationPlayer::rebuild(const Project* pProject, StringView animationPack, StringView animation, StartPlaying startPlaying)
	{
		clear();
		return addBuild(pProject, animationPack, animation, startPlaying);
	}

	//================================================================================
	bool AnimationPlayer::addBuild(const Project* pProject, StringView animationPack, StringView animationName, StartPlaying startPlaying)
	{
		auto pAnimationController = std::make_unique<AnimationController>();
		if (not(pAnimationController->build(pProject, animationPack, animationName)))
		{
			return false;
		}
		m_animationControllers.emplace_back(std::move(pAnimationController));
		m_pProject = pProject;
		if (m_currentAnimationIndex < 0)
		{
			m_currentAnimationIndex = 0;
			resetFrame();
			if (startPlaying)
			{
				play();
			}
		}
		return true;
	}

	//================================================================================
	bool AnimationPlayer::addBuild(const AnimationBuildInfo& buildInfo, StartPlaying startPlaying)
	{
		for (const auto& packInfo : buildInfo.packInfomations)
		{
			for (const auto& animName : packInfo.animationNames)
			{
				if (not(addBuild(buildInfo.pProject, packInfo.animationPackName, animName, startPlaying)))
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
		if (not(isReady()) or m_isStop)
		{
			return;
		}
		auto* pAnimationController = m_animationControllers[m_currentAnimationIndex].get();
		if (pAnimationController == nullptr)
		{
			return;
		}
		m_time += delta;
		m_frame = calcFrameByTime();
		if (isEnd())
		{
			if (m_isLoop)
			{
				resetFrame();
			}
			else
			{
				stop();
				return;
			}
		}
		pAnimationController->setFrame(m_frame);
		pAnimationController->apply();
	}

	//================================================================================
	void AnimationPlayer::draw(double x, double y) const
	{
		m_drawer.draw(getCurrentAnimationController(), x, y);
	}

	//================================================================================
	void AnimationPlayer::draw(const Vec2& pos) const
	{
		m_drawer.draw(getCurrentAnimationController(), pos.x, pos.y);
	}

	//================================================================================
	void AnimationPlayer::drawAt(double centerX, double centerY) const
	{
		m_drawer.drawAt(getCurrentAnimationController(), centerX, centerY);
	}

	//================================================================================
	void AnimationPlayer::drawAt(const Vec2& center) const
	{
		m_drawer.drawAt(getCurrentAnimationController(), center.x, center.y);
	}

	//================================================================================
	void AnimationPlayer::drawPivot(double x, double y) const
	{
		m_drawer.drawPivot(getCurrentAnimationController(), x, y);
	}

	//================================================================================
	void AnimationPlayer::drawPivot(const Vec2& pos) const
	{
		m_drawer.drawPivot(getCurrentAnimationController(), pos.x, pos.y);
	}

	//================================================================================
	bool AnimationPlayer::changeAnimation(StringView animationPackName, StringView animationName, SameChange sameChange, ChangeAnimationSetting changeAnimationSetting)
	{
		if (const auto* pCurrentController = getCurrentAnimationController())
		{
			const auto* pCurrentAnimationPack = pCurrentController->getCurrentAnimationPack();
			const auto* pCurrentAnimation = pCurrentController->getCurrentAnimation();
			if (pCurrentAnimationPack == nullptr
				or pCurrentAnimation == nullptr)
			{
				return false;
			}
			if (pCurrentAnimationPack->name == animationPackName
				and pCurrentAnimation->name == animationName)
			{
				if (not(sameChange))
				{
					return false;
				}
			}
		}
		for (size_t i = 0; i < m_animationControllers.size(); i++)
		{
			const auto* pAnimationController = m_animationControllers[i].get();
			if (pAnimationController == nullptr)
			{
				continue;
			}
			const auto* pAnimationPack = pAnimationController->getCurrentAnimationPack();
			const auto* pAnimation = pAnimationController->getCurrentAnimation();
			if (pAnimationPack == nullptr or pAnimation == nullptr)
			{
				continue;
			}
			if (pAnimationPack->name == animationPackName
				and pAnimation->name == animationName)
			{
				m_currentAnimationIndex = static_cast<int32>(i);
				resetFrame();
				return true;
			}
		}
		switch (changeAnimationSetting)
		{
		case ChangeAnimationSetting::None:
			return false;
		case ChangeAnimationSetting::AddBuild:
			if (not(addBuild(m_pProject, animationPackName, animationName, StartPlaying::No)))
			{
				return false;
			}
			return changeAnimation(animationPackName, animationName);
		case ChangeAnimationSetting::Rebuild:
			if (not(rebuild(m_pProject, animationPackName, animationName, StartPlaying::No)))
			{
				return false;
			}
			return changeAnimation(animationPackName, animationName);
		default:
			break;
		}
		return false;
	}

	//================================================================================
	bool AnimationPlayer::changeAnimation(StringView animationName, SameChange sameChange, ChangeAnimationSetting changeAnimationSetting)
	{
		const StringView& packName = getCurrentAnimationPackName();
		if (packName.isEmpty())
		{
			return false;
		}
		return changeAnimation(packName, animationName, sameChange, changeAnimationSetting);
	}

	//================================================================================
	bool AnimationPlayer::changeAnimation(int32 index, SameChange sameChange)
	{
		if (m_currentAnimationIndex == index and not(sameChange))
		{
			return false;
		}
		if (index < 0 or static_cast<int32>(m_animationControllers.size()) <= index)
		{
			return false;
		}
		m_currentAnimationIndex = index;
		return true;
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
	void AnimationPlayer::play()
	{
		m_isStop = false;
	}

	//================================================================================
	void AnimationPlayer::stop()
	{
		m_isStop = true;
	}

	//================================================================================
	void AnimationPlayer::resetFrame()
	{
		int32 startFrame = getCurrentAnimationStartFrame();
		if (startFrame < 0)
		{
			return;
		}
		setFrame(startFrame);
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
		const int32 safeFrame = Clamp(frame, getCurrentAnimationStartFrame(), getCurrentAnimationEndFrame());
		m_frame = safeFrame;
		m_time = static_cast<double>(safeFrame) / fps;
	}

	//================================================================================
	void AnimationPlayer::setTime(SecondsF seconds)
	{
		m_time = seconds.count();
		m_frame = calcFrameByTime();
	}

	//================================================================================
	const Array<std::unique_ptr<AnimationController>>& AnimationPlayer::getAnimationControllers() const noexcept
	{
		return m_animationControllers;
	}

	//================================================================================
	const AnimationController* const AnimationPlayer::getCurrentAnimationController() const noexcept
	{
		if (not(isReady()))
		{
			return nullptr;
		}
		return m_animationControllers[m_currentAnimationIndex].get();
	}

	//================================================================================
	int32 AnimationPlayer::getCurrentAnimationIndex() const noexcept
	{
		return m_currentAnimationIndex;
	}

	//================================================================================
	double AnimationPlayer::getTime() const noexcept
	{
		return m_time;
	}

	//================================================================================
	int32 AnimationPlayer::getFrame() const noexcept
	{
		return m_frame;
	}

	//================================================================================
	Size AnimationPlayer::getCanvasSize() const noexcept
	{
		if (const auto* pCurrentController = getCurrentAnimationController())
		{
			return pCurrentController->getCanvasSize();
		}
		return Size::Zero();
	}

	//================================================================================
	int32 AnimationPlayer::getCurrentAnimationFPS() const noexcept
	{
		if (const auto* pCurrentController = getCurrentAnimationController())
		{
			return pCurrentController->getFPS();
		}
		return 0;
	}

	//================================================================================
	int32 AnimationPlayer::getCurrentAnimationFrameCount() const noexcept
	{
		if (const auto* pCurrentController = getCurrentAnimationController())
		{
			return pCurrentController->getFrameCount();
		}
		return 0;
	}

	//================================================================================
	int32 AnimationPlayer::getCurrentAnimationStartFrame() const noexcept
	{
		if (const auto* pCurrentController = getCurrentAnimationController())
		{
			return pCurrentController->getStartFrame();
		}
		return -1;
	}

	//================================================================================
	int32 AnimationPlayer::getCurrentAnimationEndFrame() const noexcept
	{
		if (const auto* pCurrentController = getCurrentAnimationController())
		{
			return pCurrentController->getEndFrame();
		}
		return -1;
	}

	//================================================================================
	StringView AnimationPlayer::getCurrentAnimationPackName() const noexcept
	{
		if (const auto* pCurrentController = getCurrentAnimationController())
		{
			if (const auto* pAnimationPack = pCurrentController->getCurrentAnimationPack())
			{
				return pAnimationPack->name;
			}
		}
		return U"";
	}

	//================================================================================
	StringView AnimationPlayer::getCurrentAnimationName() const noexcept
	{
		if (const auto* pCurrentController = getCurrentAnimationController())
		{
			if (const auto* pAnimation = pCurrentController->getCurrentAnimation())
			{
				return pAnimation->name;
			}
		}
		return U"";
	}

	//================================================================================
	bool AnimationPlayer::isStopped() const noexcept
	{
		return m_isStop;
	}

	//================================================================================
	bool AnimationPlayer::isPlaying() const noexcept
	{
		if (not(isReady()))
		{
			return false;
		}
		return not(m_isStop);
	}

	//================================================================================
	bool AnimationPlayer::isEnd() const noexcept
	{
		if (const auto* pCurrentController = getCurrentAnimationController())
		{
			if (const auto* pAnimationSetting = pCurrentController->getAnimationSetting())
			{
				return (pAnimationSetting->endFrame < m_frame);
			}
		}
		return true;
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
	AnimationPlayer::operator bool() const noexcept
	{
		return isReady();
	}

	//================================================================================
	int32 AnimationPlayer::calcFrameByTime() const noexcept
	{
		return static_cast<int32>(m_time * getCurrentAnimationFPS());
	}

}
