#pragma once
#include <Siv3D.hpp>
#include "Setting/SSProjectSetting.hpp"
#include "../AnimationPack/SSAnimationPack.hpp"
#include "../Cellmap/SSCellmap.hpp"
#include "../Effect/SSEffect.hpp"
#include "../SequencePack/SSSequencePack.hpp"
#include "../Sound/SSSound.hpp"

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

		/// @brief プロジェクトの設定
		ProjectSetting m_setting;

		/// @brief アニメーションパック
		Array<AnimationPack> m_animationPacks;

		/// @brief セルマップ
		Array<Cellmap> m_cellmaps;

		/// @brief エフェクト
		Array<Effect> m_effects;

		/// @brief シーケンス
		Array<SequencePack> m_sequencePacks;

		/// @brief サウンド
		Array<Sound> m_sounds;
	};
}
