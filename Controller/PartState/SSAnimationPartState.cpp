#include "SSAnimationPartState.hpp"
#include "../SSAnimationController.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	const Mat4x4& AnimationPartState::getWorldMatrix() const
	{
		if (useLocalScale)
		{
			return localModelMatrix;
		}
		return modelMatrix;
	}

	//================================================================================
	Mat4x4& AnimationPartState::getWorldMatrixRaw()
	{
		if (useLocalScale)
		{
			return localModelMatrix;
		}
		return modelMatrix;
	}

	//================================================================================
	void AnimationPartState::updateVertices()
	{
		Buffer2D* pBufferRaw = pBuffer2D.get();
		if (pBufferRaw == nullptr
			or pAnimationModelPart == nullptr)
		{
			return;
		}

		const auto& partType = pAnimationModelPart->partType;
		Float2 pivot = Float2::Zero();

		if (partType == PartType::Shape)
		{
			cellmapTextureInfo.pCell = nullptr;
		}
		if (partType == PartType::Text)
		{

		}
		if (partType == PartType::Nineslice)
		{

		}

		if (cellmapTextureInfo.pCell != nullptr
			and cellmapTextureInfo.pTexture != nullptr)
		{
			// セルに設定された原点オフセットを適用する
			// ※セルの原点は中央が0,0で＋が右上方向になっている
			const Float2& cellPivot = cellmapTextureInfo.pCell->pivot;
			float cpx = cellPivot.x + 0.5f;
			pivot.x = cpx * size.x;
			// 上が＋で入っているのでここで反転する。
			float cpy = -cellPivot.y + 0.5f;
			pivot.y = cpy * size.y;

			// UV値を設定する
			const auto& textureSize = cellmapTextureInfo.pTexture->size();
			const auto& cellPos = cellmapTextureInfo.pCell->rect.pos + (uvTranslate * textureSize);
			const auto& cellSize = cellmapTextureInfo.pCell->rect.size;
			RectF cellRect =
			{
				static_cast<double>(cellPos.x), static_cast<double>(cellPos.y),
				static_cast<double>(cellSize.x), static_cast<double>(cellSize.y)
			};
			cellRect = cellRect.scaled(static_cast<double>(uvScale.x), static_cast<double>(uvScale.y));
			cellRect.x /= static_cast<double>(textureSize.x);
			cellRect.y /= static_cast<double>(textureSize.y);
			cellRect.w /= static_cast<double>(textureSize.x);
			cellRect.h /= static_cast<double>(textureSize.y);
			Float2 uvs[] =
			{
				Float2{ cellRect.x, cellRect.y },
				Float2{ cellRect.x + cellRect.w, cellRect.y },
				Float2{ cellRect.x, cellRect.y + cellRect.h },
				Float2{ cellRect.x + cellRect.w, cellRect.y + cellRect.h },
			};
			// 上下入れ替え
			if (isImageFlipVertical)
			{
				uvs[0].y = uvs[2].y;
				uvs[2].y = uvs[1].y;
				uvs[1].y = uvs[0].y;
				uvs[3].y = uvs[2].y;
			}
			// 左右入れ替え
			if (isImageFlipHorizontal)
			{
				uvs[0].x = uvs[1].x;
				uvs[1].x = uvs[2].x;
				uvs[2].x = uvs[0].x;
				uvs[3].x = uvs[1].x;
			}
			for (int32 i = 0; i < 4; i++)
			{
				pBufferRaw->vertices[i].tex = uvs[i];
			}

			// uvRotation
			if (uvRotation != 0.0f)
			{
				const float uvRot = Math::ToRadians(uvRotation);
				const float cosRot = Math::Cos(uvRot);
				const float sinRot = Math::Sin(uvRot);
				const float matRot[4] = { cosRot, -sinRot, sinRot, cosRot };
				const Float2 offset = { (cellPos + cellSize * 0.5f) / textureSize };
				for (int32 i = 0; i < 4; i++)
				{
					Float2 uv = pBufferRaw->vertices[i].tex - offset;
					uv = Float2{ uv.x * matRot[0] + uv.y * matRot[2], uv.x * matRot[1] + uv.y * matRot[3] };
					pBufferRaw->vertices[i].tex = uv + offset;
				}
			}
		}
		else
		{
			// セルが無いパーツでも原点が中央に来るようにする。
			pivot = size * 0.5f;
		}

		// 次に原点オフセットアニメの値を足す
		pivot.x += pivotOffset.x * size.x;
		pivot.y += -pivotOffset.y * size.y;

		const float startX = -pivot.x;
		const float endX = startX + size.x;
		const float startY = +pivot.y;
		const float endY = startY - size.y;

		const Float2 vtxPos[] =
		{
			Float2{ startX, startY }, Float2{ endX, startY },
			Float2{ startX, endY }, Float2{ endX, endY }
		};

		const Mat4x4& worldMatrix = getWorldMatrix();
		const auto& vtxOfs = vertexTrans.trans;
		for (int32 i = 0; i < 4; i++)
		{
			const Float3 transformPoint = worldMatrix.transformPoint(Float3{ vtxPos[i] + vtxOfs[i], 0.0f });
			pBufferRaw->vertices[i].pos.x = transformPoint.x;
			pBufferRaw->vertices[i].pos.y = -transformPoint.y;
		}

		// 当たり判定の設定
		updateBounds();

		// α値の設定
		{
			float tmpAlpha = alpha;
			if (useLocalAlpha)
			{
				tmpAlpha = localAlpha; // ローカルアルファ値優先
			}
			for (int32 i = 0; i < 4; i++)
			{
				pBufferRaw->vertices[i].color.w = tmpAlpha;
			}
		}
	}

	//================================================================================
	void AnimationPartState::updateBounds()
	{
		if (pBoundsValue.get() == nullptr
			or pBuffer2D.get() == nullptr)
		{
			return;
		}

		if (pAnimationModelPart->boundsType == BoundsType::Quad)
		{
			Quad& quad = std::get<Quad>(*pBoundsValue.get());
			quad.set(pBuffer2D->vertices[0].pos, pBuffer2D->vertices[1].pos, pBuffer2D->vertices[3].pos, pBuffer2D->vertices[2].pos);
			return;
		}

		Vec2 topLeft, bottomRight;
		topLeft = bottomRight = pBuffer2D->vertices[0].pos;
		for (int32 i = 1; i < 4; i++)
		{
			const auto& pos = pBuffer2D->vertices[i].pos;
			if (pos.x < topLeft.x)
			{
				topLeft.x = pos.x;
			}
			if (pos.y < topLeft.y)
			{
				topLeft.y = pos.y;
			}
			if (bottomRight.x < pos.x)
			{
				bottomRight.x = pos.x;
			}
			if (bottomRight.y < pos.y)
			{
				bottomRight.y = pos.y;
			}
		}
		switch (pAnimationModelPart->boundsType)
		{
		case BoundsType::AABB:
		{
			RectF& rect = std::get<RectF>(*pBoundsValue.get());
			rect.set(topLeft, bottomRight - topLeft);
			return;
		}
		case BoundsType::Circle:
		{
			Circle& circle = std::get<Circle>(*pBoundsValue.get());
			circle.setR(boundingRadius);
			circle.setCenter(RectF{ topLeft, bottomRight - topLeft }.center());
			return;
		}
		case BoundsType::CircleScaleMin:
		{
			Circle& circle = std::get<Circle>(*pBoundsValue.get());
			circle.setR(boundingRadius * scale.minComponent());
			circle.setCenter(RectF{ topLeft, bottomRight - topLeft }.center());
			return;
		}
		case BoundsType::CircleScaleMax:
		{
			Circle& circle = std::get<Circle>(*pBoundsValue.get());
			circle.setR(boundingRadius * scale.maxComponent());
			circle.setCenter(RectF{ topLeft, bottomRight - topLeft }.center());
			return;
		}
		case BoundsType::Quad:
		case BoundsType::None:
		default:
			return;
		}
	}

	//================================================================================
	void AnimationPartState::updateMatrix()
	{
		// 親があれば継承
		if (pParent != nullptr)
		{
			modelMatrix *= pParent->modelMatrix;
		}

		// サンプルがこの順番でかけていたので・・・個人的には見慣れない・・・
		const Mat4x4 trans = Mat4x4::Translate(position);
		const Mat4x4 rotateX = Mat4x4::RotateX(Math::ToRadians(rotation.x));
		const Mat4x4 rotateY = Mat4x4::RotateY(Math::ToRadians(-rotation.y));
		const Mat4x4 rotateZ = Mat4x4::RotateZ(Math::ToRadians(rotation.z));
		const Mat4x4 matScale = Mat4x4::Scale(Float3{ scale, 1.0f });
		if (useLocalScale)
		{
			const Mat4x4 matLocalScale = Mat4x4::Scale(Float3{ scale * localScale, 1.0f });
			localModelMatrix = modelMatrix;
			localModelMatrix = trans * localModelMatrix;
			localModelMatrix = rotateX * localModelMatrix;
			localModelMatrix = rotateY * localModelMatrix;
			localModelMatrix = rotateZ * localModelMatrix;
			localModelMatrix = matLocalScale * localModelMatrix;
		}
		{
			modelMatrix = trans * modelMatrix;
			modelMatrix = rotateX * modelMatrix;
			modelMatrix = rotateY * modelMatrix;
			modelMatrix = rotateZ * modelMatrix;
			modelMatrix = matScale * modelMatrix;
		}
	}

}
