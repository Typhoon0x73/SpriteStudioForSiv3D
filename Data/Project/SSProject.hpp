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

		/// @brief プロジェクトデータをリセットし、空の状態にします。
		void clear();

		/// @brief プロジェクトデータが空の状態か判定します。
		/// @return 初期の状態、空の状態であればtrue, 何かしらデータがあればfalseが返ります。
		[[nodiscard]]
		bool isEmpty() const;

		/// @brief プロジェクトの設定を設定します。
		/// @param setting 設定するプロジェクトの設定
		void setProjectSetting(const ProjectSetting& setting);

		/// @brief アニメーションパックをm_animationPacksの末尾に追加します。
		/// @param animPack 追加するアニメーションパック
		void addAnimationPack(const AnimationPack& animPack);

		/// @brief セルマップをm_cellmapsの末尾に追加します。
		/// @param cellmap 追加するセルマップ
		void addCellmap(const Cellmap& cellmap);

		/// @brief エフェクトをm_effectsの末尾に追加します。
		/// @param effect 追加するエフェクト
		void addEffect(const Effect& effect);

		/// @brief シーケンスパックをm_sequencePacksの末尾に追加します。
		/// @param sequencePack 追加するシーケンスパック
		void addSequencePack(const SequencePack& sequencePack);

		/// @brief サウンドをm_soundsの末尾に追加します。
		/// @param sound 追加するサウンド
		void addSound(const Sound& sound);

	private:

		/// @brief プロジェクトの設定
		ProjectSetting m_setting;

		/// @brief アニメーションパック
		Array<AnimationPack> m_animationPacks;

		/// @brief セルマップ
		Array<Cellmap> m_cellmaps;

		/// @brief エフェクト
		Array<Effect> m_effects;

		/// @brief シーケンスパック
		Array<SequencePack> m_sequencePacks;

		/// @brief サウンド
		Array<Sound> m_sounds;

	};
}
