#pragma once
#include <Siv3D.hpp>
#include "../Data/Project/SSProject.hpp"
#include "PartState/SSAnimationPartState.hpp"

namespace s3d::SpriteStudio
{
	/// @brief アニメーション制御
	class AnimationController
	{
	public:

		/// @brief コンストラクタ
		[[nodiscard]]
		AnimationController();

		/// @brief プロジェクトとアニメーションを指定してアニメーション制御を作成します。
		/// @param pProject 参照するプロジェクト
		/// @param animationPackName 再生するアニメーションを含むアニメーションパック名
		/// @param animationName 再生するアニメーション名
		[[nodiscard]]
		AnimationController(const Project* pProject, StringView animationPackName, StringView animationName);

		/// @brief デストラクタ
		virtual ~AnimationController();

		/// @brief 生成された内部値や参照を消します。
		void clear();

		/// @brief プロジェクトとアニメーションを指定してアニメーション制御を作成します。
		/// @param pProject 参照するプロジェクト
		/// @param animationPackName 再生するアニメーションを含むアニメーションパック名
		/// @param animationName 再生するアニメーション名
		[[nodiscard]]
		bool build(const Project* pProject, StringView animationPackName, StringView animationName);

		/// @brief 設定されたフレームの状態に更新します。
		void apply();

		/// @brief 指定したフレームに設定します。
		/// @param frame 指定するフレーム
		void setFrame(int32 frame);

		/// @brief 再生制御できる状態かを判定します。
		/// @return 再生可能の場合 true を返します。 それ以外 false
		[[nodiscard]]
		bool isReady() const noexcept;

		/// @brief プロジェクトの設定を取得します。
		/// @return プロジェクトの設定が返ります。プロジェクトが存在しない場合、nullptr
		const ProjectSetting* const getProjectSetting() const noexcept;

		/// @brief パーツリストを取得します。
		/// @return パーツリスト
		[[nodiscard]]
		Array<std::unique_ptr<AnimationPartState>>& getPartStatesRaw() noexcept;

	private:

		/// @brief 指定アニメーションデータから各アニメーションパーツを作成します。
		/// @param pProject プロジェクトの参照
		/// @param pAnimationPack アニメーションパックの参照
		/// @param pAnimation アニメーションの参照
		/// @return 成功で true を返します。それ以外 false
		[[nodiscard]]
		bool createAnimationPartStates(const Project* pProject, const AnimationPack* pAnimationPack, const Animation* pAnimation);

		/// @brief ソートリストをソート済みの状態にします。
		void partsSort();

		/// @brief ボーン影響値設定を行います。
		/// @param pAnimationPack アニメーションパックの参照
		/// @param バインド成功で true を返す。 それ以外 false
		bool meshPartBind(const AnimationPack* pAnimationPack);

		/// @brief 更新する前に初期化しておきたいパラメータを初期化します。
		/// @param pPartState 初期化するパーツ
		void initPartState(AnimationPartState* pPartState);

		/// @brief パーツの状態を指定したフレームの状態に更新します。
		/// @param pPartState 更新するパーツ
		/// @param frame 更新するフレーム
		void updatePartState(AnimationPartState* pPartState, int32 frame);

	private:

		/// @brief プロジェクトデータの参照
		const Project* m_pProject;

		/// @brief 再生中アニメーションパックのキャッシュ
		const AnimationPack* m_pCurrentAnimationPack;

		/// @brief 再生中アニメーションのキャッシュ
		const Animation* m_pCurrentAnimation;

		/// @brief 使用するアニメーション設定のキャッシュ
		const AnimationSetting* m_pAnimationSetting;

		/// @brief フレーム
		int32 m_frame;

		/// @brief 適応が必要な状態かのフラグ
		bool m_isNeedApply;

		/// @brief 各パーツの状態
		Array<std::unique_ptr<AnimationPartState>> m_partStates;

		/// @brief 描画用のソート済みパーツリスト
		Array<AnimationPartState*> m_sortPartStates;

		/// @brief メッシュパーツリスト
		Array<AnimationPartState*> m_meshPartStates;

		/// @brief ボーンパーツリスト
		Array<AnimationPartState*> m_bonePartStates;
	};
}
