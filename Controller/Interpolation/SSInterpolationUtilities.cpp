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

}
