#include "SSProject.hpp"
#include "../../Loader/SSLoadUtilities.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	Project::Project()
		: m_setting{}
		, m_animationPacks{}
		, m_cellmaps{}
		, m_effects{}
		, m_sequencePacks{}
		, m_sounds{}
	{
	}

	//================================================================================
	Project::Project(FilePathView path)
		: Project{}
	{
		// ファイルから読み込み
		if (not(Utilities::LoadProject(path, this)))
		{
			clear();
		}
	}

	//================================================================================
	Project::~Project()
	{
	}

	//================================================================================
	void Project::clear()
	{
		m_setting = ProjectSetting();
		m_animationPacks.clear();
		m_cellmaps.clear();
		m_effects.clear();
		m_sequencePacks.clear();
		m_sounds.clear();
	}

	//================================================================================
	bool Project::isEmpty() const noexcept
	{
		if (
			(m_animationPacks.size() != 0)
			or (m_cellmaps.size() != 0)
			or (m_effects.size() != 0)
			or (m_sequencePacks.size() != 0)
			or (m_sounds.size() != 0)
			)
		{
			return false;
		}
		return true;
	}

	//================================================================================
	void Project::setProjectSetting(const ProjectSetting& setting)
	{
		m_setting = setting;
	}

	//================================================================================
	void Project::addAnimationPack(const AnimationPack& animPack)
	{
		m_animationPacks.emplace_back(animPack);
	}

	//================================================================================
	void Project::addCellmap(const Cellmap& cellmap)
	{
		m_cellmaps.emplace_back(cellmap);
	}

	//================================================================================
	void Project::addEffect(const Effect& effect)
	{
		m_effects.emplace_back(effect);
	}

	//================================================================================
	void Project::addSequencePack(const SequencePack& sequencePack)
	{
		m_sequencePacks.emplace_back(sequencePack);
	}

	//================================================================================
	void Project::addSound(const Sound& sound)
	{
		m_sounds.emplace_back(sound);
	}

	//================================================================================
	const Array<Cellmap>& Project::getCellmaps() const noexcept
	{
		return m_cellmaps;
	}

	//================================================================================
	ResourcePack& Project::getResourcePackRaw() noexcept
	{
		return m_resourcePack;
	}

	//================================================================================
	const ResourcePack& Project::getResourcePack() const noexcept
	{
		return m_resourcePack;
	}

	//================================================================================
	Project::operator bool() const noexcept
	{
		return not(isEmpty());
	}

}
