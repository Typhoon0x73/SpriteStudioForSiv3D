#pragma once
#include <Siv3D.hpp>
#include "Setting/SSProjectSetting.hpp"
#include "../AnimationPack/SSAnimationPack.hpp"
#include "../Cellmap/SSCellmap.hpp"
#include "../Effect/SSEffect.hpp"
#include "../SequencePack/SSSequencePack.hpp"

namespace s3d::SpriteStudio
{
	/// @brief スプライトスタジオのプロジェクトデータ
	class Project
	{
	public:

		/// @brief コンストラクタ
		[[nodiscard]]
		Project();

		/// @brief ファイルからプロジェクトデータを作成します。
		/// @param path ファイルパス
		[[nodiscard]]
		explicit Project(FilePathView path);

		/// @brief デストラクタ
		virtual ~Project();

	private:

		ProjectSetting m_setting;

		Array<AnimationPack> m_animationPacks;

		Array<Cellmap> m_cellmaps;

		Array<Effect> m_effects;

		Array<SequencePack> m_sequencePacks;
	};
}
