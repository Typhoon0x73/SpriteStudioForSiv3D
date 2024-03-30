#include "SSInterpolationUtilities.hpp"

namespace s3d::SpriteStudio::Utilities
{
	namespace
	{
		//================================================================================
		/// @brief エルミート補間
		float Hermite(float leftValue, float rightValue, float nowRate, const AnimationCurve& curve)
		{
			float rate2 = nowRate * nowRate;
			float rate3 = rate2 * nowRate;
			float result =
				(2 * rate3 - 3 * rate2 + 1) * leftValue +
				(-2 * rate3 + 3 * rate2) * rightValue +
				(rate3 - 2 * rate2 + nowRate) * (curve.startValue - leftValue) +
				(rate3 - rate2) * (curve.endValue - rightValue);
			return result;
		}

		//================================================================================
		/// @brief ベジェ補間
		float Bezier(float leftValue, float rightValue, float nowRate, const AnimationCurve& curve, float startKeyFrame, float endKeyFrame)
		{
			//値が変化しない場合は左キーを補間値とする
			if ((leftValue == rightValue) and (curve.startValue == 0.0f) and (curve.endValue == 0.0f))
			{
				return leftValue;
			}


			float currentPos = (endKeyFrame - startKeyFrame) * nowRate + startKeyFrame;

			float ret = rightValue;
			float currentCalc = 0.5f;
			float calcRange = 0.5f;

			float temp1 = 0.0f, temp2 = 0.0f, temp3 = 0.0f, currentX = 0.0f;

			for (int32 loopCount = 0; loopCount < 8; loopCount++)
			{
				// ループ数が増えると、精度が向上する。
				temp1 = 1.0f - currentCalc;
				temp2 = temp1 * temp1;
				temp3 = temp2 * temp1;
				currentX = (temp3 * startKeyFrame) +
					(3 * temp2 * currentCalc * (curve.startFrame + startKeyFrame)) +
					(3 * temp1 * currentCalc * currentCalc * (curve.endFrame + endKeyFrame)) +
					(currentCalc * currentCalc * currentCalc * endKeyFrame);

				calcRange /= 2.0f;
				if (currentPos < currentX)
				{
					currentCalc -= calcRange;
				}
				else
				{
					currentCalc += calcRange;
				}
			}

			// 最後に現在の値で計算
			temp1 = 1.0f - currentCalc;
			temp2 = temp1 * temp1;
			temp3 = temp2 * temp1;
			ret = (temp3 * leftValue) +
				(3 * temp2 * currentCalc * (curve.startValue + leftValue)) +
				(3 * temp1 * currentCalc * currentCalc * (curve.endValue + rightValue)) +
				(currentCalc * currentCalc * currentCalc * rightValue);

			return ret;
		}
	}

	//================================================================================
	float Interpolation(InterpolationType type, float easingRate, float nowRate, float leftValue, float rightValue, const AnimationCurve& curve, float startKeyFrame, float endKeyFrame)
	{
		float result = leftValue;
		switch (type)
		{
		case InterpolationType::Linear:
		{
			result = Math::Lerp(leftValue, rightValue, nowRate);
			break;
		}
		case InterpolationType::Hermite:
		{
			result = Hermite(leftValue, rightValue, nowRate, curve);
			break;
		}
		case InterpolationType::Bezier:
		{
			result = Bezier(leftValue, rightValue, nowRate, curve, startKeyFrame, endKeyFrame);
			break;
		}
		case InterpolationType::Acceleration:
		{
			float rate2 = nowRate * nowRate;
			result = leftValue + (rate2 * (rightValue - leftValue));
			break;
		}
		case InterpolationType::Deceleration:
		{
			float invRate = 1 - nowRate;
			float rate2 = 1 - invRate * invRate;
			result = leftValue + (rate2 * (rightValue - leftValue));
			break;
		}
		case InterpolationType::EaseIn:
		{
			double t = static_cast<double>(Math::Pow(nowRate, easingRate));
			result = static_cast<float>(EaseIn(Easing::Linear, leftValue, rightValue, t));
			break;
		}
		case InterpolationType::EaseOut:
		{
			double t = static_cast<double>(Math::Pow(nowRate, 1 / easingRate));
			result = static_cast<float>(EaseOut(Easing::Linear, leftValue, rightValue, t));
			break;
		}
		case InterpolationType::EaseInOut:
		{
			double t = nowRate * 2;
			if (t < 1)
			{
				t = 0.5 * static_cast<double>(Math::Pow(t, easingRate));
			}
			else
			{
				t = 1.0 - 0.5 * static_cast<double>(Math::Pow(2 - t, easingRate));
			}
			result = static_cast<float>(EaseInOut(Easing::Linear, leftValue, rightValue, t));
			break;
		}
		case InterpolationType::EaseExponentialIn:
		{
			result = static_cast<float>(EaseIn(Easing::Expo, leftValue, rightValue, nowRate));
			break;
		}
		case InterpolationType::EaseExponentialOut:
		{
			result = static_cast<float>(EaseOut(Easing::Expo, leftValue, rightValue, nowRate));
			break;
		}
		case InterpolationType::EaseExponentialInOut:
		{
			result = static_cast<float>(EaseInOut(Easing::Expo, leftValue, rightValue, nowRate));
			break;
		}
		case InterpolationType::EaseSineIn:
		{
			result = static_cast<float>(EaseIn(Easing::Sine, leftValue, rightValue, nowRate));
			break;
		}
		case InterpolationType::EaseSineOut:
		{
			result = static_cast<float>(EaseOut(Easing::Sine, leftValue, rightValue, nowRate));
			break;
		}
		case InterpolationType::EaseSineInOut:
		{
			result = static_cast<float>(EaseInOut(Easing::Sine, leftValue, rightValue, nowRate));
			break;
		}
		case InterpolationType::EaseElasticIn:
		{
			double t = detail::ElasticAP(nowRate, 1.0, easingRate);
			result = static_cast<float>(EaseIn(Easing::Linear, leftValue, rightValue, t));
			break;
		}
		case InterpolationType::EaseElasticOut:
		{
			double t = detail::ElasticAP(nowRate, 1.0, easingRate);
			result = static_cast<float>(EaseOut(Easing::Linear, leftValue, rightValue, t));
			break;
		}
		case InterpolationType::EaseElasticInOut:
		{
			double t = detail::ElasticAP(nowRate, 1.0, easingRate);
			result = static_cast<float>(EaseInOut(Easing::Linear, leftValue, rightValue, t));
			break;
		}
		case InterpolationType::EaseBounceIn:
		{
			result = static_cast<float>(EaseIn(Easing::Bounce, leftValue, rightValue, nowRate));
			break;
		}
		case InterpolationType::EaseBounceOut:
		{
			result = static_cast<float>(EaseOut(Easing::Bounce, leftValue, rightValue, nowRate));
			break;
		}
		case InterpolationType::EaseBounceInOut:
		{
			result = static_cast<float>(EaseInOut(Easing::Bounce, leftValue, rightValue, nowRate));
			break;
		}
		case InterpolationType::EaseBackIn:
		{
			result = static_cast<float>(EaseIn(Easing::Back, leftValue, rightValue, nowRate));
			break;
		}
		case InterpolationType::EaseBackOut:
		{
			result = static_cast<float>(EaseOut(Easing::Back, leftValue, rightValue, nowRate));
			break;
		}
		case InterpolationType::EaseBackInOut:
		{
			result = static_cast<float>(EaseInOut(Easing::Back, leftValue, rightValue, nowRate));
			break;
		}

			// 何もしない組
		case InterpolationType::Invalid:
		case InterpolationType::None:
		default:
			break;
		}
		return result;
	}

	//================================================================================
	void InterpolationValue([[maybe_unused]] int32 frame, const AnimationKeyFrame* pLeft, [[maybe_unused]] const AnimationKeyFrame* pRight, AttributeValueCell& out)
	{
		if (pLeft == nullptr)
		{
			// 左値すらない。
			return;
		}

		if (not(std::holds_alternative<AttributeValueCell>(pLeft->attributeVariantValue)))
		{
			// セル値ではない？？？
			return;
		}

		// 左値をそのまま採用。
		out = std::get<AttributeValueCell>(pLeft->attributeVariantValue);
	}

	//================================================================================
	void InterpolationValue(int32 frame, const AnimationKeyFrame* pLeft, const AnimationKeyFrame* pRight, AttributeValuePartsColor& out)
	{
		if (pLeft == nullptr)
		{
			// 左値すらない。
			return;
		}

		const auto* pLeftValue = std::get_if<AttributeValuePartsColor>(&(pLeft->attributeVariantValue));
		if (pLeftValue == nullptr)
		{
			// パーツカラー値ではない？？？
			return;
		}

		if (pRight == nullptr)
		{
			// 右値がないので左値をそのまま採用。
			out = *pLeftValue;
			return;
		}
		const auto* pRightValue = std::get_if<AttributeValuePartsColor>(&(pRight->attributeVariantValue));
		if (pRightValue == nullptr)
		{
			// パーツカラー値ではない？？？
			return;
		}

		float now = 0.0f;
		const int32 range = pRight->frame - pLeft->frame;
		if (0 < range)
		{
			now = static_cast<float>(frame - pLeft->frame) / range;
		}
		const float startFrame = static_cast<float>(pLeft->frame);
		const float endFrame = static_cast<float>(pRight->frame);

		// 初期化しておく。
		for (auto& color : out.colors)
		{
			color.rgba = Color::Zero();
			color.rate = 0.0f;
		}
		out.blendType = pLeftValue->blendType;
		out.target = ColorBlendTarget::Vertex;

		// 補間
		const float easingRate = pLeft->easingRate;
		const AnimationCurve& curve = pLeft->curve;
		now = Interpolation(pLeft->interpolationType, easingRate, now, 0.0f, 1.0f, curve, startFrame, endFrame);

		if (pLeftValue->target == ColorBlendTarget::Vertex)
		{
			if (pRightValue->target == ColorBlendTarget::Vertex)
			{
				// 両方とも頂点カラー
				for (int32 i = 0; i < 4; i++)
				{
					auto& leftColorRaw = pLeftValue->colors[i];
					auto& rightColorRaw = pRightValue->colors[i];
					out.colors[i].rate = Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rate, rightColorRaw.rate, curve, startFrame, endFrame), 0.0f, 1.0f);
					out.colors[i].rgba.a = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.a, rightColorRaw.rgba.a, curve, startFrame, endFrame), 0.0f, 255.0f));
					out.colors[i].rgba.r = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.r, rightColorRaw.rgba.r, curve, startFrame, endFrame), 0.0f, 255.0f));
					out.colors[i].rgba.g = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.g, rightColorRaw.rgba.g, curve, startFrame, endFrame), 0.0f, 255.0f));
					out.colors[i].rgba.b = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.b, rightColorRaw.rgba.b, curve, startFrame, endFrame), 0.0f, 255.0f));
				}
			}
			else
			{
				// 左は頂点、右は単色
				auto& rightColorRaw = pRightValue->colors[0];
				for (int32 i = 0; i < 4; i++)
				{
					auto& leftColorRaw = pLeftValue->colors[i];
					out.colors[i].rate = Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rate, rightColorRaw.rate, curve, startFrame, endFrame), 0.0f, 1.0f);
					out.colors[i].rgba.a = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.a, rightColorRaw.rgba.a, curve, startFrame, endFrame), 0.0f, 255.0f));
					out.colors[i].rgba.r = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.r, rightColorRaw.rgba.r, curve, startFrame, endFrame), 0.0f, 255.0f));
					out.colors[i].rgba.g = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.g, rightColorRaw.rgba.g, curve, startFrame, endFrame), 0.0f, 255.0f));
					out.colors[i].rgba.b = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.b, rightColorRaw.rgba.b, curve, startFrame, endFrame), 0.0f, 255.0f));
				}
			}
		}
		else
		{
			if (pRightValue->target == ColorBlendTarget::Vertex)
			{
				// 左は単色、右は頂点カラー
				auto& leftColorRaw = pLeftValue->colors[0];
				for (int32 i = 0; i < 4; i++)
				{
					auto& rightColorRaw = pRightValue->colors[i];
					out.colors[i].rate = Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rate, rightColorRaw.rate, curve, startFrame, endFrame), 0.0f, 1.0f);
					out.colors[i].rgba.a = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.a, rightColorRaw.rgba.a, curve, startFrame, endFrame), 0.0f, 255.0f));
					out.colors[i].rgba.r = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.r, rightColorRaw.rgba.r, curve, startFrame, endFrame), 0.0f, 255.0f));
					out.colors[i].rgba.g = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.g, rightColorRaw.rgba.g, curve, startFrame, endFrame), 0.0f, 255.0f));
					out.colors[i].rgba.b = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.b, rightColorRaw.rgba.b, curve, startFrame, endFrame), 0.0f, 255.0f));
				}
			}
			else
			{
				// 両方とも単色
				auto& leftColorRaw = pLeftValue->colors[0];
				auto& rightColorRaw = pRightValue->colors[0];
				out.colors[0].rate = Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rate, rightColorRaw.rate, curve, startFrame, endFrame), 0.0f, 1.0f);
				out.colors[0].rgba.a = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.a, rightColorRaw.rgba.a, curve, startFrame, endFrame), 0.0f, 255.0f));
				out.colors[0].rgba.r = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.r, rightColorRaw.rgba.r, curve, startFrame, endFrame), 0.0f, 255.0f));
				out.colors[0].rgba.g = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.g, rightColorRaw.rgba.g, curve, startFrame, endFrame), 0.0f, 255.0f));
				out.colors[0].rgba.b = static_cast<uint8>(Clamp(Interpolation(InterpolationType::Linear, easingRate, now, leftColorRaw.rgba.b, rightColorRaw.rgba.b, curve, startFrame, endFrame), 0.0f, 255.0f));
				out.target = ColorBlendTarget::Whole;
			}
		}
	}

	//================================================================================
	void InterpolationValue(int32 frame, const AnimationKeyFrame* pLeft, const AnimationKeyFrame* pRight, AttributeValueShader& out)
	{
		if (pLeft == nullptr)
		{
			// 左値すらない。
			return;
		}

		const auto* pLeftValue = std::get_if<AttributeValueShader>(&(pLeft->attributeVariantValue));
		if (pLeftValue == nullptr)
		{
			// シェーダー値ではない？？？
			return;
		}

		if (pRight == nullptr)
		{
			// 右値がないので左値をそのまま採用。
			out = *pLeftValue;
			return;
		}
		const auto* pRightValue = std::get_if<AttributeValueShader>(&(pRight->attributeVariantValue));
		if (pRightValue == nullptr)
		{
			// シェーダー値ではない？？？
			return;
		}

		float now = 0.0f;
		const int32 range = pRight->frame - pLeft->frame;
		if (0 < range)
		{
			now = static_cast<float>(frame - pLeft->frame) / range;
		}
		const float startFrame = static_cast<float>(pLeft->frame);
		const float endFrame = static_cast<float>(pRight->frame);

		// 初期化しておく。
		out.id = pLeftValue->id;
		for (auto& param : out.param)
		{
			param = 0.0f;
		}

		// 補間
		const float easingRate = pLeft->easingRate;
		const AnimationCurve& curve = pLeft->curve;
		now = Interpolation(pLeft->interpolationType, easingRate, now, 0.0f, 1.0f, curve, startFrame, endFrame);

		for (int32 i = 0; i < 4; i++)
		{
			out.param[i] = Interpolation(InterpolationType::Linear, easingRate, now, pLeftValue->param[i], pRightValue->param[i], curve, startFrame, endFrame);
		}
	}

	//================================================================================
	void InterpolationValue(int32 frame, const AnimationKeyFrame* pLeft, const AnimationKeyFrame* pRight, AttributeValueVertexTranslate& out)
	{
		if (pLeft == nullptr)
		{
			// 左値すらない。
			return;
		}

		const auto* pLeftValue = std::get_if<AttributeValueVertexTranslate>(&(pLeft->attributeVariantValue));
		if (pLeftValue == nullptr)
		{
			// 頂点変形値ではない？？？
			return;
		}

		if (pRight == nullptr)
		{
			// 右値がないので左値をそのまま採用。
			out = *pLeftValue;
			return;
		}
		const auto* pRightValue = std::get_if<AttributeValueVertexTranslate>(&(pRight->attributeVariantValue));
		if (pRightValue == nullptr)
		{
			// 頂点変形値ではない？？？
			return;
		}

		float now = 0.0f;
		const int32 range = pRight->frame - pLeft->frame;
		if (0 < range)
		{
			now = static_cast<float>(frame - pLeft->frame) / range;
		}
		const float startFrame = static_cast<float>(pLeft->frame);
		const float endFrame = static_cast<float>(pRight->frame);

		// 初期化しておく。
		for (auto& trans : out.trans)
		{
			trans = Float2::Zero();
		}

		// 補間
		const float easingRate = pLeft->easingRate;
		const AnimationCurve& curve = pLeft->curve;

		now = Interpolation(pLeft->interpolationType, easingRate, now, 0.0f, 1.0f, curve, startFrame, endFrame);

		for (int32 i = 0; i < 4; i++)
		{
			out.trans[i].x = Interpolation(InterpolationType::Linear, easingRate, now, pLeftValue->trans[i].x, pRightValue->trans[i].x, curve, startFrame, endFrame);
			out.trans[i].y = Interpolation(InterpolationType::Linear, easingRate, now, pLeftValue->trans[i].y, pRightValue->trans[i].y, curve, startFrame, endFrame);
		}
	}

	//================================================================================
	void InterpolationValue([[maybe_unused]] int32 frame, const AnimationKeyFrame* pLeft, [[maybe_unused]] const AnimationKeyFrame* pRight, AttributeValueUser& out)
	{
		if (pLeft == nullptr)
		{
			// 左値すらない。
			return;
		}

		if (not(std::holds_alternative<AttributeValueUser>(pLeft->attributeVariantValue)))
		{
			// ユーザー値ではない？？？
			return;
		}

		// 左値をそのまま採用。
		out = std::get<AttributeValueUser>(pLeft->attributeVariantValue);
	}

	//================================================================================
	void InterpolationValue([[maybe_unused]] int32 frame, const AnimationKeyFrame* pLeft, [[maybe_unused]] const AnimationKeyFrame* pRight, AttributeValueSignal& out)
	{
		if (pLeft == nullptr)
		{
			// 左値すらない。
			return;
		}

		if (not(std::holds_alternative<AttributeValueSignal>(pLeft->attributeVariantValue)))
		{
			// シグナル値ではない？？？
			return;
		}

		// 左値をそのまま採用。
		out = std::get<AttributeValueSignal>(pLeft->attributeVariantValue);
	}

	//================================================================================
	void InterpolationValue([[maybe_unused]] int32 frame, const AnimationKeyFrame* pLeft, [[maybe_unused]] const AnimationKeyFrame* pRight, AttributeValueInstance& out)
	{
		if (pLeft == nullptr)
		{
			// 左値すらない。
			return;
		}

		if (not(std::holds_alternative<AttributeValueInstance>(pLeft->attributeVariantValue)))
		{
			// インスタンス値ではない？？？
			return;
		}

		// 左値をそのまま採用。
		out = std::get<AttributeValueInstance>(pLeft->attributeVariantValue);
	}

	//================================================================================
	void InterpolationValue([[maybe_unused]] int32 frame, const AnimationKeyFrame* pLeft, [[maybe_unused]] const AnimationKeyFrame* pRight, AttributeValueEffect& out)
	{
		if (pLeft == nullptr)
		{
			// 左値すらない。
			return;
		}

		if (not(std::holds_alternative<AttributeValueEffect>(pLeft->attributeVariantValue)))
		{
			// エフェクト値ではない？？？
			return;
		}

		// 左値をそのまま採用。
		out = std::get<AttributeValueEffect>(pLeft->attributeVariantValue);
	}

	//================================================================================
	void InterpolationValue([[maybe_unused]] int32 frame, const AnimationKeyFrame* pLeft, [[maybe_unused]] const AnimationKeyFrame* pRight, AttributeValueAudio& out)
	{
		if (pLeft == nullptr)
		{
			// 左値すらない。
			return;
		}

		if (not(std::holds_alternative<AttributeValueAudio>(pLeft->attributeVariantValue)))
		{
			// サウンド値ではない？？？
			return;
		}

		// 左値をそのまま採用。
		out = std::get<AttributeValueAudio>(pLeft->attributeVariantValue);
	}

	//================================================================================
	void InterpolationValue([[maybe_unused]] int32 frame, const AnimationKeyFrame* pLeft, [[maybe_unused]] const AnimationKeyFrame* pRight, AttributeValueTextureChange& out)
	{
		if (pLeft == nullptr)
		{
			// 左値すらない。
			return;
		}

		if (not(std::holds_alternative<AttributeValueTextureChange>(pLeft->attributeVariantValue)))
		{
			// 画像切り替え値ではない？？？
			return;
		}

		// 左値をそのまま採用。
		out = std::get<AttributeValueTextureChange>(pLeft->attributeVariantValue);
	}

	//================================================================================
	void InterpolationValue(int32 frame, const AnimationKeyFrame* pLeft, const AnimationKeyFrame* pRight, AttributeValueDeform& out)
	{
		out.verticesChanges.clear();

		if (pLeft == nullptr)
		{
			// 左値すらない。
			return;
		}

		const auto* pLeftValue = std::get_if<AttributeValueDeform>(&(pLeft->attributeVariantValue));
		if (pLeftValue == nullptr)
		{
			// デフォーム値ではない？？？
			return;
		}

		if (pRight == nullptr)
		{
			// 右値がないので左値をそのまま採用。
			out = *pLeftValue;
			return;
		}
		const auto* pRightValue = std::get_if<AttributeValueDeform>(&(pRight->attributeVariantValue));
		if (pRightValue == nullptr)
		{
			// デフォーム値ではない？？？
			return;
		}

		float now = 0.0f;
		const int32 range = pRight->frame - pLeft->frame;
		if (0 < range)
		{
			now = static_cast<float>(frame - pLeft->frame) / range;
		}
		const float startFrame = static_cast<float>(pLeft->frame);
		const float endFrame = static_cast<float>(pRight->frame);

		const float easingRate = pLeft->easingRate;
		const AnimationCurve& curve = pLeft->curve;
		now = Interpolation(pLeft->interpolationType, easingRate, now, 0.0f, 1.0f, curve, startFrame, endFrame);

		// スタートとエンドの頂点数を比較し、多い方に合わせる(足りない部分は0とみなす)
		const size_t leftVerticesCount = pLeftValue->verticesChanges.size();
		const size_t rightVerticesCount = pRightValue->verticesChanges.size();
		const size_t pointNum = Max(leftVerticesCount, rightVerticesCount);

		out.verticesChanges.reserve(pointNum);
		for (size_t i = 0; i < pointNum; i++)
		{
			const float leftX = ((i < leftVerticesCount) ? pLeftValue->verticesChanges[i].x : 0.0f);
			const float leftY = ((i < leftVerticesCount) ? pLeftValue->verticesChanges[i].y : 0.0f);
			const float rightX = ((i < rightVerticesCount) ? pRightValue->verticesChanges[i].x : 0.0f);
			const float rightY = ((i < rightVerticesCount) ? pRightValue->verticesChanges[i].y : 0.0f);
			const float x = Interpolation(InterpolationType::Linear, easingRate, now, leftX, rightX, curve, startFrame, endFrame);
			const float y = Interpolation(InterpolationType::Linear, easingRate, now, leftY, rightY, curve, startFrame, endFrame);
			out.verticesChanges.emplace_back(Float2{ x, y });
		}
	}

}
