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

		/// @brief 指定アニメーションの中から、探したいラベル名を指定して見つかったフレーム値を返します。
		/// @param pAnim 指定アニメーション
		/// @param label ラベル名
		/// @return 見つかった場合、見つけたフレーム。それ以外 -1
		[[nodiscard]]
		static int32 FindAnimationLabelToFrame(const Animation* pAnimation, StringView label);

		/// @brief コンストラクタ
		[[nodiscard]]
		AnimationController();

		/// @brief プロジェクトとアニメーションを指定してアニメーション制御を作成します。
		/// @param pProject 参照するプロジェクト
		/// @param animationPackName 再生するアニメーションを含むアニメーションパック名
		/// @param animationName 再生するアニメーション名
		[[nodiscard]]
		explicit AnimationController(const Project* pProject, StringView animationPackName, StringView animationName);

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
		[[nodiscard]]
		const ProjectSetting* const getProjectSetting() const noexcept;

		/// @brief アニメーション設定のキャンバスサイズを取得します。
		/// @return 再生できるアニメーションが設定されていなければ Size::Zero() を返します。
		[[nodiscard]]
		Size getCanvasSize() const noexcept;

		/// @brief アニメーション設定のFPSを取得します。
		/// @return 再生できるアニメーションが設定されていなければ 0 を返します。
		[[nodiscard]]
		int32 getFPS() const noexcept;

		/// @brief アニメーション設定のフレーム数を取得します。
		/// @return 再生できるアニメーションが設定されていなければ 0 を返します。
		[[nodiscard]]
		int32 getFrameCount() const noexcept;

		/// @brief アニメーションの開始フレームを取得します。
		/// @return 再生できるアニメーションが設定されていなければ -1 を返します。
		[[nodiscard]]
		int32 getStartFrame() const noexcept;

		/// @brief アニメーションの終了フレームを取得します。
		/// @return 再生できるアニメーションが設定されていなければ -1 を返します。
		[[nodiscard]]
		int32 getEndFrame() const noexcept;

		/// @brief パーツリストを取得します。
		/// @return パーツリスト
		[[nodiscard]]
		Array<std::unique_ptr<AnimationPartState>>& getPartStatesRaw() noexcept;

		/// @brief パーツリストを取得します。
		/// @return パーツリスト
		[[nodiscard]]
		const Array<std::unique_ptr<AnimationPartState>>& getPartStates() const noexcept;

		/// @brief 描画用ソート済みパーツリストを取得します。
		/// @return 描画用ソート済みパーツリスト
		[[nodiscard]]
		const Array<AnimationPartState*>& getDrawPartStates() const noexcept;

		/// @brief 参照しているプロジェクトデータへのポインタを取得します。
		/// @return 存在しなければ nullptr
		[[nodiscard]]
		const Project* const getProject() const noexcept;

		/// @brief 現在制御しているアニメーションを含むパックを取得します。
		/// @return 存在しなければ nullptr
		[[nodiscard]]
		const AnimationPack* const getCurrentAnimationPack() const noexcept;

		/// @brief 現在制御しているアニメーションを取得します。
		/// @return 存在しなければ nullptr
		[[nodiscard]]
		const Animation* const getCurrentAnimation() const noexcept;

		/// @brief 現在制御しているアニメーションの設定を取得します。
		/// @return 存在しなければ nullptr
		[[nodiscard]]
		const AnimationSetting* const getAnimationSetting() const noexcept;

	private:

		/// @brief インスタンスアニメーションからのラベルのフレーム値を取得します。
		/// @param label ラベル名
		/// @param offset オフセット
		/// @param pAnimation 参照アニメーション
		/// @param pSetting 参照アニメーション設定
		/// @return フレーム値
		[[nodiscard]]
		static int32 AnimationLabelToFrame(StringView label, int32 offset, const Animation* pAnimation, const AnimationSetting* pSetting);

		/// @brief セットアップパーツ、アニメーションパーツから最初にあるセル参照データを取得します。
		/// @param pSetupPart セットアップパーツ
		/// @param pAnimationPart アニメーションパーツ
		/// @param out 出力先
		/// @return セル参照データを見つけた場合は true を返します。それ以外 false
		[[nodiscard]]
		bool getFirstCellValue(const AnimationPart* pSetupPart, const AnimationPart* pAnimationPart, AttributeValueCell& out);

		/// @brief セル参照データからセル情報、画像へのポインタを取得します。
		/// @param pProject 参照するプロジェクトデータ
		/// @param refCell 検索する参照データ
		/// @param pOut 出力先
		/// @return セル、画像ともにポインタ設定できた場合は true を返します。それ以外 false
		[[nodiscard]]
		bool getCellTexture(const Project* pProject, const AttributeValueCell& refCell, CellmapTextureInfo& pOut);

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
		[[nodiscard]]
		bool meshPartBind(const AnimationPack* pAnimationPack);

		/// @brief 更新する前に初期化しておきたいパラメータを初期化します。
		/// @param pPartState 初期化するパーツ
		void initState(AnimationPartState* pPartState);

		/// @brief パーツの状態を指定したフレームの状態に更新します。
		/// @param pPartState 更新するパーツ
		/// @param frame 更新するフレーム
		void updatePartState(AnimationPartState* pPartState, int32 frame);

		/// @brief パーツの状態を指定フレームの前後キーフレームデータから補間計算を行います。
		/// @param pPartState 更新するパーツ
		/// @param attributes 参照するアトリビュート
		/// @param frame 指定フレーム
		void updatePartStateAttributes(AnimationPartState* pPartState, const Array<AnimationAttribute>& attributes, int32 frame);

		/// @brief インスタンスパーツを指定フレームの状態に更新します。
		/// @param pPartState 更新するパーツ
		/// @param frame 指定フレーム
		void updateInstance(AnimationPartState* pPartState, int32 frame);

		/// @brief エフェクトパーツを指定フレームの状態に更新します。
		/// @param pPartState 更新するパーツ
		/// @param frame 指定フレーム
		void updateEffect(AnimationPartState* pPartState, int32 frame);

		/// @brief メッシュの状態を更新します。
		/// @param pPartState 更新するパーツ
		void updateMesh(AnimationPartState* pPartState);

		/// @brief 頂点のローカルオフセット値を算出します。
		/// @param pPartState 参照するパーツ
		/// @param index 頂点インデックス
		/// @return ローカルオフセット値
		[[nodiscard]]
		Float2 calcOffsetLocalVertexPos(const AnimationPartState* pPartState, int32 index) const;

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

		/// @brief 直前のフレーム
		int32 m_prevFrame;

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

		/// @brief 非表示キーを見つけているか
		bool m_isFoundKeyHide;

		/// @brief サイズXキーを見つけているか
		bool m_isFoundKeySizeX;

		/// @brief サイズYキーを見つけているか
		bool m_isFoundKeySizeY;

		/// @brief インスタンスパーツ用の非表示フラグ
		bool m_isInstancePartsHide;

		/// @brief エフェクトのシードへ影響する値
		int32 m_effectSeedOffset;
	};
}
