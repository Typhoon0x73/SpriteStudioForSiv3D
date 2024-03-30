#pragma once
#include <Siv3D.hpp>
#include "../../Data/AnimationPack/Model/SSAnimationModelPart.hpp"
#include "../../Data/AnimationPack/Animation/SSAnimationPart.hpp"

namespace s3d::SpriteStudio::Utilities
{
	/// @brief 左右値から補間します。
	/// @param type 補完方法
	/// @param easingRate イージングレート
	/// @param nowRate 今の時間割合（0.0f ~ 1.0f）
	/// @param leftValue 左値
	/// @param rightValue 右値
	/// @param curve 曲線計算用パラメータ
	/// @param startKeyFrame ベジェ用開始キーフレーム値
	/// @param endKeyFrame ベジェ用終了キーフレーム値
	/// @return 補間結果
	float Interpolation(InterpolationType type, float easingRate, float nowRate, float leftValue, float rightValue, const AnimationCurve& curve, float startKeyFrame, float endKeyFrame);

	/// @brief セル値補間（左キー採用）
	void InterpolationValue([[maybe_unused]] int32 frame, const AnimationKeyFrame* pLeft, [[maybe_unused]] const AnimationKeyFrame* pRight, AttributeValueCell& out);

	/// @brief パーツカラー値補間
	void InterpolationValue(int32 frame, const AnimationKeyFrame* pLeft, const AnimationKeyFrame* pRight, AttributeValuePartsColor& out);
	
	/// @brief シェーダーカラー値補間
	void InterpolationValue(int32 frame, const AnimationKeyFrame* pLeft, const AnimationKeyFrame* pRight, AttributeValueShader& out);

	/// @brief 頂点変形値補間
	void InterpolationValue(int32 frame, const AnimationKeyFrame* pLeft, const AnimationKeyFrame* pRight, AttributeValueVertexTranslate& out);

	/// @brief ユーザーデータ値補間（左キー採用）
	void InterpolationValue([[maybe_unused]] int32 frame, const AnimationKeyFrame* pLeft, [[maybe_unused]] const AnimationKeyFrame* pRight, AttributeValueUser& out);

	/// @brief シグナル値補間（左キー採用）
	void InterpolationValue([[maybe_unused]] int32 frame, const AnimationKeyFrame* pLeft, [[maybe_unused]] const AnimationKeyFrame* pRight, AttributeValueSignal& out);

	/// @brief インスタンス値補間（左キー採用）
	void InterpolationValue([[maybe_unused]] int32 frame, const AnimationKeyFrame* pLeft, [[maybe_unused]] const AnimationKeyFrame* pRight, AttributeValueInstance& out);

	/// @brief エフェクト値補間（左キー採用）
	void InterpolationValue([[maybe_unused]] int32 frame, const AnimationKeyFrame* pLeft, [[maybe_unused]] const AnimationKeyFrame* pRight, AttributeValueEffect& out);

	/// @brief サウンド値補間（左キー採用）
	void InterpolationValue([[maybe_unused]] int32 frame, const AnimationKeyFrame* pLeft, [[maybe_unused]] const AnimationKeyFrame* pRight, AttributeValueAudio& out);

	/// @brief 画像切り替え値補間（左キー採用）
	void InterpolationValue([[maybe_unused]] int32 frame, const AnimationKeyFrame* pLeft, [[maybe_unused]] const AnimationKeyFrame* pRight, AttributeValueTextureChange& out);

	/// @brief デフォーム値補間
	void InterpolationValue(int32 frame, const AnimationKeyFrame* pLeft, const AnimationKeyFrame* pRight, AttributeValueDeform& out);

	/// @brief 指定フレームの左右キーフレームデータから補完します。
	/// @tparam T float, int32, boolはこの関数を利用。その他は別関数実装。
	/// @param frame 指定フレーム
	/// @param pLeft 左キーフレーム値
	/// @param pRight 右キーフレーム値
	/// @param out 補間対象出力先
	template< class T > requires std::is_convertible_v<T, float>
	inline void InterpolationValue(int32 frame, const AnimationKeyFrame* pLeft, const AnimationKeyFrame* pRight, T& out);

	/// @brief 指定フレームの左右キーフレームデータから補完します。
	/// @tparam T 各アトリビュートに対応したデータ型
	/// @param frame 指定フレーム
	/// @param pSetupPart セットアップパーツ
	/// @param attribute アトリビュート
	/// @param out 補間対象出力先
	/// @return 補完した場合は指定されたフレームを返します。補完されなかった場合、採用されたキーのフレームを返します。
	template< class T >
	inline int32 GetKeyValue(int32 frame, const AnimationPart* pSetupPart, const AnimationAttribute& attribute, T& out);

#include "SSInterpolationUtilities.ipp"
}
