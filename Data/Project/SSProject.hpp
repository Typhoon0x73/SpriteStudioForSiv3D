#pragma once
#include <Siv3D.hpp>
#include "Setting/SSProjectSetting.hpp"
#include "../AnimationPack/SSAnimationPack.hpp"
#include "../Cellmap/SSCellmap.hpp"
#include "../Effect/SSEffect.hpp"
#include "../SequencePack/SSSequencePack.hpp"
#include "../Sound/SSSound.hpp"
#include "../ResourcePack/SSResourcePack.hpp"

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
		bool isEmpty() const noexcept;

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

		/// @brief プロジェクト設定を取得します。
		/// @return プロジェクト設定
		[[nodiscard]]
		const ProjectSetting& getProjectSetting() const noexcept;

		/// @brief セルマップリストを取得します。
		/// @return セルマップリスト
		[[nodiscard]]
		const Array<Cellmap>& getCellmaps() const noexcept;

		/// @brief 素材データを参照で取得します。
		/// @return 素材データ
		[[nodiscard]]
		ResourcePack& getResourcePackRaw() noexcept;

		/// @brief 素材データを取得します。
		/// @return 素材データ
		[[nodiscard]]
		const ResourcePack& getResourcePack() const noexcept;

		/// @brief アニメーションパック名からアニメーションパックを探します。
		/// @param animationPackName アニメーションパック名
		/// @return 見つかればアニメーションパックのポインタが返ります。 それ以外 nullptr
		[[nodiscard]]
		const AnimationPack* const findAnimationPack(StringView animationPackName) const;

		/// @brief アニメーションパック名,アニメーション名からアニメーションを探します。
		/// @param animationPackName アニメーションパック名
		/// @param animationName アニメーション名
		/// @return 見つかればアニメーションのポインタが返ります。 それ以外 nullptr
		[[nodiscard]]
		const Animation* const findAnimation(StringView animationPackName, StringView animationName) const;

		/// @brief 指定情報からセルを探します。
		/// @param mapId セルマップID
		/// @param cellName セル名
		/// @return 見つかった場合、セルへのポインタが返ります。それ以外 nullptr
		[[nodiscard]]
		const Cell* const findCell(int32 mapId, StringView cellName) const noexcept;

		/// @brief プロジェクトデータが空でないかを返します。
		/// @return プロジェクトデータが空でない場合 true, それ以外の場合は false
		[[nodiscard]]
		explicit operator bool() const noexcept;

	private:

		/// @brief プロジェクトの設定
		ProjectSetting m_setting{};

		/// @brief アニメーションパック
		Array<AnimationPack> m_animationPacks{};

		/// @brief セルマップ
		Array<Cellmap> m_cellmaps{};

		/// @brief エフェクト
		Array<Effect> m_effects{};

		/// @brief シーケンスパック
		Array<SequencePack> m_sequencePacks{};

		/// @brief サウンド
		Array<Sound> m_sounds{};

		/// @brief 素材データ
		ResourcePack m_resourcePack{};
	};
}
