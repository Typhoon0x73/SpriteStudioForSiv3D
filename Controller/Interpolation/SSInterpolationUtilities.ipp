
//================================================================================
template< class T > requires std::is_convertible_v<T, float>
inline void InterpolationValue(int32 frame, const AnimationKeyFrame* pLeft, const AnimationKeyFrame* pRight, T& out)
{
	if (pLeft == nullptr)
	{
		// 左キーが無い場合は何もできない。
		return;
	}
	// 左キーの値を取得
	auto* pLeftValue = std::get_if<T>(&(pLeft->attributeVariantValue));
	if (pLeftValue == nullptr)
	{
		// 値の取得ができなかった？？？
		return;
	}
	// 右キーが無い場合、左キーを設定して終了。
	if (pRight == nullptr)
	{
		out = *pLeftValue;
		return;
	}
	// 右キーの値を取得
	auto* pRightValue = std::get_if<T>(&(pRight->attributeVariantValue));
	if (pRightValue == nullptr)
	{
		// 値の取得が出来ず。
		return;
	}

	const float leftValue = static_cast<float>(*pLeftValue);
	const float rightValue = static_cast<float>(*pRightValue);

	float now = 0.0f;
	const int32 range = pRight->frame - pLeft->frame;
	if (0 < range)
	{
		now = static_cast<float>(frame - pLeft->frame) / range;
	}
	// 補間計算して、元の型に戻す。
	const float startFrame = static_cast<float>(pLeft->frame);
	const float endFrame = static_cast<float>(pRight->frame);
	out = static_cast<T>(Interpolation(pLeft->interpolationType, pLeft->easingRate, now, leftValue, rightValue, pLeft->curve, startFrame, endFrame));
}


//================================================================================
template< class T >
inline int32 GetKeyValue(int32 frame, const AnimationPart* pSetupPart, const AnimationAttribute& attribute, T& out)
{
	if (attribute.keyFrames.isEmpty())
	{
		return -1;
	}
	const auto* pLeftKey = attribute.findLeftKey(frame);

	// 左のキーがない場合は、最初のキーを探して採用する。
	if (pLeftKey == nullptr)
	{
		// セットアップパーツから最初のキーを探してみる。
		if (pSetupPart != nullptr)
		{
			for (const auto& it : pSetupPart->attributes)
			{
				if (it.kind != attribute.kind)
				{
					continue;
				}
				pLeftKey = it.getFirstKey();
				break;
			}
		}
		// セットアップになかったらこのアトリビュートから最初のキーを探してみる。
		if (pLeftKey == nullptr)
		{
			pLeftKey = attribute.getFirstKey();
		}
		// これでもない場合はデータがおかしい。
		if (pLeftKey == nullptr)
		{
			return -1;
		}

		InterpolationValue(frame, pLeftKey, nullptr, out);

		return pLeftKey->frame;
	}
	// 左キーと同値であれば、そのまま左キーを採用する。
	// 補完方法の指定が無い場合でも左キーを採用する。
	else if (
		(pLeftKey->frame == frame)
		or (pLeftKey->interpolationType == InterpolationType::None)
		)
	{
		InterpolationValue(frame, pLeftKey, nullptr, out);
		return pLeftKey->frame;
	}

	const auto* pRightKey = attribute.findRightKey(frame);
	// 右キーが無い場合も左キーを採用する。
	if (pRightKey == nullptr)
	{
		InterpolationValue(frame, pLeftKey, nullptr, out);
		return pLeftKey->frame;
	}

	// 補間計算をする。
	InterpolationValue(frame, pLeftKey, pRightKey, out);
	return frame;
}
