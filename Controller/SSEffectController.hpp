#pragma once
#include <Siv3D.hpp>
#include "Effect/Emitter/SSEffectEmitter.hpp"
#include "../Data/Effect/SSEffectModel.hpp"
#include "../Data/Project/SSProject.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	// 前方宣言
	//================================================================================
	struct AnimationPartState;

	//================================================================================
	/// @brief エフェクト制御
	class EffectController
	{
	public:

		/// @brief コンストラクタ
		[[nodiscard]]
		EffectController();

		/// @brief デストラクタ
		virtual ~EffectController();

		/// @brief 再生開始
		void play();

		/// @brief 一時停止
		void stop();

		/// @brief 現在のフレームの状態に更新
		void apply();

		/// @brief 設定されているエフェクトモデルからロードしなおします。
		void reload();

		/// @brief プロジェクトデータへの参照を設定します。
		/// @param pProject プロジェクトデータ
		void setProject(const Project* pProject);

		/// @brief エフェクトモデルを設定します。
		/// @param pEffectModel エフェクトモデル
		void setEffectModel(const EffectModel* pEffectModel);

		/// @brief 乱数シードを設定します。
		/// @param seed シード値
		void setSeed(uint32 seed);

		/// @brief シード値のオフセットを設定します。
		/// @param offset オフセット
		void setSeedOffset(int32 offset);

		/// @brief フレーム値を設定します。
		/// @param frame フレーム値
		void setFrame(int32 frame);

		/// @brief 親パーツへのリンクを設定します。
		/// @param pState パーツ
		void setParentPartState(const AnimationPartState* pState);

		/// @brief ループを設定します。
		/// @param enable Yesでループします。Noでループしません。
		void setLoop(LoopEnable enable);

	private:

		/// @brief エミッタを初期化します。
		/// @param out 初期化対象
		/// @param src 初期化データ
		void initEmitter(EffectEmitter& out, const EffectNode& src);

	private:

		/// @brief プロジェクトへの参照
		const Project* m_pProject;

		/// @brief モデルへの参照
		const EffectModel* m_pEffectModel;

		/// @brief 親パーツへの参照
		const AnimationPartState* m_pParentState;

		/// @brief ループフラグ
		bool m_isLoop;

		/// @brief 停止フラグ
		bool m_isStop;

		/// @brief 無限生成可能フラグ
		bool m_isInfinite;

		/// @brief 更新が必要かのフラグ
		bool m_isNeedApply;

		/// @brief 現在のフレーム
		int32 m_frame;

		/// @brief 
		int32 m_targetFrame;

		/// @brief 
		size_t m_effectTimeLength;

		/// @brief シード値
		uint32 m_seed;

		/// @brief 
		Float3 m_layoutPosition;

		/// @brief 
		Float2 m_layoutScale;

		/// @brief シード値のオフセット
		int32 m_seedOffset;

		/// @brief エミッタリスト
		Array<EffectEmitter> m_emitters;

		/// @brief エミッタの更新用ソート済みリスト
		Array<EffectEmitter*> m_updateEmitters;
	};
}
