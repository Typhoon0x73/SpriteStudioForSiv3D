#pragma once

namespace s3d::SpriteStudio
{
	/// @brief メッシュバインドの最大数
	constexpr unsigned long long int MeshBindBoneMax = 128;

	/// @brief アトリビュートの種類数
	constexpr unsigned long long int AttributeKindNum = 43;

	/// @brief 読み込むファイルの種類
	enum class FileType
	{
		Unknown, ///< 不明
		Xml,     ///< sspj
	};

	/// @brief パーツのソートモード
	enum class PartsSortMode
	{
		Invalid = -1, ///< 無効値
		Prio,         ///< ステートの優先順でソート
		Z,            ///< ステートのZ座標でソート
	};

	/// @brief パーツの種類
	enum class PartType
	{
		Invalid = -1, ///< 無効値
		Null,         ///< null。領域を持たない。ただし円形の当たり判定は設定可能。
		Normal,       ///< 通常パーツ。領域を持つ。画像は無くてもいい。
		Text,         ///< テキスト(予約　未実装）
		Instance,     ///< インスタンス。他アニメ、パーツへの参照。シーン編集モードの代替になるもの
		Armature,     ///< ボーンパーツ
		Effect,       ///< エフェクト
		Mesh,         ///< メッシュパーツ
		Movenode,     ///< 動作起点
		Constraint,   ///< コンストレイント
		Mask,         ///< マスク
		Joint,        ///< メッシュとボーンの関連付けパーツ
		Bonepoint,    ///< ボーンポイント
		Shape,        ///< シェイプ
		Nineslice,    ///< ナインスライス
	};

	/// @brief 当たり判定形状
	enum class BoundsType
	{
		Invalid = -1,   ///< 無効値
		None,           ///< 当たり判定として使わない。
		Quad,           ///< 自在に変形する四辺形。頂点変形など適用後の４角を結んだ領域。最も重い。
		AABB,           ///< 回転しない全体を囲む矩形で交差判定
		Circle,         ///< 真円の半径で距離により判定する
		CircleScaleMin, ///< 真円の半径で距離により判定する (スケールはx,yの最小値をとる）
		CircleScaleMax, ///< 真円の半径で距離により判定する (スケールはx,yの最大値をとる）
	};

	/// @brief 継承の種類
	enum class InheritType
	{
		Invalid = -1, ///< 無効値
		Parent,       ///< 親の継承方法をそのまま引き継ぐ
		Self,         ///< 自身がアトリビュート別に持つ継承方法を使う
	};


	/// @brief アルファブレンドの種類
	enum class BlendType
	{
		Invalid = -1, ///< 無効値
		Mix,          ///< 0 ブレンド（ミックス）
		Mul,          ///< 1 乗算
		Add,          ///< 2 加算
		Sub,          ///< 3 減算
		MulAlpha,     ///< 4 α乗算
		Screen,       ///< 5 スクリーン
		Exclusion,    ///< 6 除外
		Invert,       ///< 7 反転
	};

	/// @brief カラーブレンドキーが使用されている際のカラー適用範囲の定義
	enum class ColorBlendTarget
	{
		Invalid = -1, ///< 無効値
		Whole,        ///< 単色。全体にかける。
		Vertex,       ///< 頂点単位
	};

	/// @brief 補間の種類
	enum class InterpolationType
	{
		Invalid = -1, ///< 無効値
		None,         ///< なし
		Linear,       ///< 線形
		Hermite,      ///< エルミート
		Bezier,       ///< ベジェ
		Acceleration, ///< 加速度
		Deceleration, ///< 減速度

		// 以下各イーズ

		EaseIn,
		EaseOut,
		EaseInOut,

		EaseExponentialIn,
		EaseExponentialOut,
		EaseExponentialInOut,

		EaseSineIn,
		EaseSineOut,
		EaseSineInOut,

		EaseElasticIn,
		EaseElasticOut,
		EaseElasticInOut,

		EaseBounceIn,
		EaseBounceOut,
		EaseBounceInOut,

		EaseBackIn,
		EaseBackOut,
		EaseBackInOut,
	};

	/// @brief アトリビュートの種類
	enum class AttributeKind
	{
		Invalid = -1,  ///< 無効値
		Cell = 0,      ///< [CELL]参照セル
		PosX,          ///< [POSX]位置.X
		PosY,          ///< [POSY]位置.Y
		PosZ,          ///< [POSZ]位置.Z
		RotateX,       ///< [ROTX]回転.X
		RotateY,       ///< [ROTY]回転.Y
		RotateZ,       ///< [ROTZ]回転.Z
		ScaleX,        ///< [SCLX]スケール.X
		ScaleY,        ///< [SCLY]スケール.Y
		LocalScaleX,   ///< [LSCX]ローカルスケール.X
		LocalScaleY,   ///< [LSCY]ローカルスケール.Y
		Alpha,         ///< [ALPH]不透明度
		LocalAlpha,    ///< [LALP]ローカル不透明度
		Priority,      ///< [PRIO]優先度
		FlipH,         ///< [FLPH]左右反転(セルの原点を軸にする) ※ SSVer6非対応
		FlipV,         ///< [FLPV]上下反転(セルの原点を軸にする) ※ SSVer6非対応
		Hide,          ///< [HIDE]非表示
		PartsColor,    ///< [PCOL]パーツカラー
		VertexColor,   ///< [VCOL]カラーブレンド ※ SSVer6非対応
		Shader,        ///< [SHDR]シェーダー
		Vertex,        ///< [VERT]頂点変形
		PivotX,        ///< [PVTX]原点オフセット.X
		PivotY,        ///< [PVTY]原点オフセット.Y
		AnchorX,       ///< [ANCX]アンカーポイント.X ※ SSVer6非対応
		AnchorY,       ///< [ANCY]アンカーポイント.Y ※ SSVer6非対応
		SizeX,         ///< [SIZX]表示サイズ.X
		SizeY,         ///< [SIZY]表示サイズ.Y
		ImageFlipH,    ///< [IFLH]イメージ左右反転(常にイメージの中央を原点とする)
		ImageFlipV,    ///< [IFLV]イメージ上下反転(常にイメージの中央を原点とする)
		UVTranslateX,  ///< [UVTX]UVアニメ.移動.X
		UVTranslateY,  ///< [UVTY]UVアニメ.移動.Y
		UVRotateZ,     ///< [UVRZ]UVアニメ.回転
		UVScaleX,      ///< [UVSX]UVアニメ.スケール.X
		UVScaleY,      ///< [UVSY]UVアニメ.スケール.Y
		BoundRadius,   ///< [BNDR]当たり判定用の半径
		Mask,          ///< [MASK]マスク閾値
		User,          ///< [USER]ユーザーデータ
		Signal,        ///< [SIGN]シグナル
		InstanceParam, ///< [IPRM]インスタンスパーツパラメータ
		Effect,        ///< [EFCT]エフェクトパラメータ
		Deform,        ///< [DEFM]デフォーム用パラメータ
		Audio,         ///< [ADIO]オーディオ再生
		TextureChange, ///< [TCHG]画像差し替え

		// ※パラメータが増えた場合、AttributeKindNumも修正すること。
	};

	/// @brief IKの方向
	enum class IKRotationArrow
	{
		Invalid = -1,  ///< 無効値
		ArrowFree,     ///< 指定なし
		ClockWise,     ///< 時計回り
		AntiClockWise, ///< 反時計回り
	};

	/// @brief エフェクトノードの種類
	enum class EffectNodeType
	{
		Invalid = -1, ///< 無効値
		Root,         ///< ルート
		Emitter,      ///< エミッタ
		Particle,     ///< パーティクル
	};

	/// @brief シグナルの種類
	enum class SignalType
	{
		Invalid = -1, ///< 無効値
		None,         ///< 無し
		Index,        ///< リスト
		Integer,      ///< 整数
		Float,        ///< 浮動小数
		Text,         ///< テキスト
	};

	/// @brief 基点
	enum class Anchor : unsigned char
	{
		None         = 0xFF,
		LeftTop      = 0x00,
		LeftCenter   = 0x01,
		LeftBottom   = 0x02,
		CenterTop    = 0x10,
		Center       = 0x11,
		CenterBottom = 0x12,
		RightTop     = 0x20,
		RightCenter  = 0x21,
		RightBottom  = 0x22,
	};

	/// @brief アライメント
	enum class Alignment : unsigned short
	{
		Left    = 0x0001,
		Right   = 0x0002,
		HCenter = 0x0004,
		Top     = 0x0020,
		Bottom  = 0x0040,
		VCenter = 0x0080,
	};

	/// @brief シェイプの種類
	enum class ShapeType
	{
		Invalid = -1, ///< 無効値
		Unknown,      ///< 不明
		Triangle,     ///< 三角形
		Rectangle,    ///< 矩形
		Arrow,        ///< 矢印
		Star,         ///< 星形
	};
}
