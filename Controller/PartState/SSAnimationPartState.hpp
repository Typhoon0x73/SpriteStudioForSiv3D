#pragma once
#include <Siv3D.hpp>
#include "../../Data/AnimationPack/Model/SSAnimationModelPart.hpp"
#include "../../Data/AnimationPack/Animation/SSAnimationPart.hpp"
#include "../../Data/Cellmap/SSCellmap.hpp"
#include "SSAnimationPartStateInstance.hpp"
#include "SSAnimationPartStateMesh.hpp"

namespace s3d::SpriteStudio
{
	/// @brief パーツごとに変わる値
	using AnimationPartStateValueVarint = std::variant<
		std::monostate
		, AnimationPartStateInstance
		, AnimationPartStateMesh
	>;

	/// @brief セルマップ、画像をまとめて扱う構造体
	struct CellmapTextureInfo
	{
		const Cell* pCell{ nullptr };
		const Cellmap* pCellmap{ nullptr };
		const Texture* pTexture{ nullptr };
	};

	/// @brief アニメーションパーツの状態
	struct AnimationPartState
	{
		/// @brief 親へのポインタ
		AnimationPartState* pParent{ nullptr };

		/// @brief モデルパーツへのポインタ
		const AnimationModelPart* pAnimationModelPart{ nullptr };

		/// @brief セットアップアニメーションパーツへのポインタ
		const AnimationPart* pSetupAnimationPart{ nullptr };

		/// @brief アニメーションパーツへのポインタ
		const AnimationPart* pAnimationPart{ nullptr };

		/// @brief 描画用バッファ
		std::unique_ptr<Buffer2D> pBuffer2D{ nullptr };

		/// @brief パーツのモデルマトリックス
		Mat4x4 modelMatrix{ Mat4x4::Identity() };

		/// @brief ローカル値を適応したパーツのモデルマトリックス
		Mat4x4 localModelMatrix{ Mat4x4::Identity() };

		/// @brief 参照セル値
		AttributeValueCell refCell{};

		/// @brief 座標
		Float3 position{ Float3::Zero() };

		/// @brief 回転 degree
		Float3 rotation{ Float3::Zero() };

		/// @brief 拡縮
		Float2 scale{ Float2::One() };

		/// @brief このパーツにのみ適応する拡縮
		Float2 localScale{ Float2::One() };

		/// @brief アルファ値の継承率
		float alphaInheritRate{ 1.0f };

		/// @brief アルファ値
		float alpha{ 1.0f };

		/// @brief このパーツのみに適応するアルファ値
		float localAlpha{ 1.0f };

		/// @brief 優先度
		int32 priority{ 0 };

		/// @brief 非表示フラグ
		bool isHide{ false };

		/// @brief 原点のオフセット。旧SSの原点は左上基準にしたこの値に相当する。0,0が中央+0.5,+0.5が右上になる。参照セルがある場合はセルの原点に+する=オフセット扱いになる。
		Float2 pivotOffset{ Float2::Zero() };

		/// @brief サイズ
		Float2 size{ Float2::One() };

		/// @brief 水平方向の反転
		bool isImageFlipHorizontal{ false };

		/// @brief 垂直方向の反転
		bool isImageFlipVertical{ false };

		/// @brief uvの移動値
		Float2 uvTranslate{ Float2::Zero() };

		/// @brief uvのZ軸回転値 degree
		float uvRotation{ 0.0f };

		/// @brief uvの拡縮
		Float2 uvScale{ Float2::One() };

		/// @brief 当たり判定用半径
		float boundingRadius{ 0.0f };

		/// @brief マスク影響値
		int32 maskThreshold{ 0 };

		/// @brief パーツカラー値
		AttributeValuePartsColor partsColor{};

		/// @brief シェーダー値
		AttributeValueShader shaderParam{};

		/// @brief 頂点変形値
		AttributeValueVertexTranslate vertexTrans{};

		/// @brief エフェクト値
		AttributeValueEffect effectParam{};

		/// @brief エフェクトシード値
		int32 effectSeed{ 0 };

		/// @brief 
		int32 effectTime{ 0 };

		/// @brief 
		float effectTimeTotal{ 0.0f };

		/// @brief ユーザーデータ値
		AttributeValueUser userParam{};

		/// @brief シグナル値
		AttributeValueSignal signalParam{};

		/// @brief インスタンス値
		AttributeValueInstance instanceParam{};

		/// @brief デフォーム値
		AttributeValueDeform deformParam{};

		/// @brief サウンド値
		AttributeValueAudio audioParam{};

		/// @brief 画像切り替え値
		AttributeValueTextureChange textureChangeParam{};

		/// @brief 生成時の配列インデックス
		int32 index{ -1 };

		/// @brief 当たり判定用パラメータ
		std::unique_ptr<BoundsValueVariant> pBoundsValue{ nullptr };

		/// @brief パーツカラーを使用するフラグ
		bool usePartsColor{ false };

		/// @brief シェーダーを使用するフラグ
		bool useShader{ false };

		/// @brief 頂点変形を使用するフラグ
		bool useVertexTrans{ false };

		/// @brief ローカルアルファ値を使用するフラグ
		bool useLocalAlpha{ false };

		/// @brief ローカルスケールを使用するフラグ
		bool useLocalScale{ false };

		/// @brief デフォーム値を使用するフラグ
		bool useDeform{ false };

		/// @brief 描画用参照セル、画像
		CellmapTextureInfo cellmapTextureInfo{};

		/// @brief パーツごとに変わる値
		AnimationPartStateValueVarint partValue{ std::monostate{} };

		/// @brief ワールドマトリクスを取得します。
		/// @return ローカル影響値ありでlocalModelMatrixを返します。 それ以外 modelMatrix
		const Mat4x4& getWorldMatrix() const;

		/// @brief ワールドマトリクスを取得します。
		/// @return ローカル影響値ありでlocalModelMatrixを返します。 それ以外 modelMatrix
		Mat4x4& getWorldMatrixRaw();

		/// @brief 頂点を更新します。
		void updateVertices();

		/// @brief 当たり判定を更新します。
		void updateBounds();

		/// @brief マトリクスを更新します。
		void updateMatrix();

	};
}
