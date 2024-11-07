#include "SSAnimationDrawer.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	BlendState AnimationDrawer::GetBlendState(BlendType blendType)
	{
		switch (blendType)
		{
		case BlendType::Mix:       return BlendState{ true, Blend::SrcAlpha    , Blend::InvSrcAlpha , BlendOp::Add };
		case BlendType::Mul:       return BlendState{ true, Blend::Zero        , Blend::SrcColor    , BlendOp::Add };
		case BlendType::Add:       return BlendState{ true, Blend::SrcAlpha    , Blend::One         , BlendOp::Add };
		case BlendType::Sub:       return BlendState{ true, Blend::SrcAlpha    , Blend::One         , BlendOp::RevSubtract, Blend::Zero, Blend::DestAlpha };
		case BlendType::MulAlpha:  return BlendState{ true, Blend::DestColor   , Blend::InvSrcAlpha , BlendOp::Add };
		case BlendType::Screen:    return BlendState{ true, Blend::InvDestColor, Blend::One         , BlendOp::Add };
		case BlendType::Exclusion: return BlendState{ true, Blend::InvDestColor, Blend::InvDestColor, BlendOp::Add };
		case BlendType::Invert:    return BlendState{ true, Blend::InvDestColor, Blend::Zero        , BlendOp::Add };

		default: break;
		}
		return BlendState{ BlendState::Default2D };
	}

	//================================================================================
	AnimationDrawer::AnimationDrawer()
		: m_pController{ nullptr }
		, m_boundsColor{ Palette::White }
		, m_boundsFrameColor{ Palette::White }
		, m_isBoundsDrawFill{ true }
		, m_isBoundsDrawFrame{ true }
	{
	}

	//================================================================================
	AnimationDrawer::AnimationDrawer(const AnimationController* pController)
		: AnimationDrawer{}
	{
		setAnimationController(pController);
	}

	//================================================================================
	AnimationDrawer::~AnimationDrawer()
	{
	}

	//================================================================================
	void AnimationDrawer::resetDefault()
	{
		m_pController = nullptr;
		m_boundsColor = Palette::White;
		m_boundsFrameColor = Palette::White;
	}

	//================================================================================
	void AnimationDrawer::setAnimationController(const AnimationController* pController)
	{
		m_pController = pController;
	}

	//================================================================================
	void AnimationDrawer::setBoundsDraw(BoundsDraw fill, BoundsDraw frame)
	{
		m_isBoundsDrawFill = fill.getBool();
		m_isBoundsDrawFrame = frame.getBool();
	}

	//================================================================================
	void AnimationDrawer::setBoundsColor(const ColorF& color)
	{
		m_boundsColor = color;
	}

	//================================================================================
	void AnimationDrawer::setBoundsColor(const ColorF& fillColor, const ColorF& frameColor)
	{
		m_boundsColor = fillColor;
		m_boundsFrameColor = frameColor;
	}

	//================================================================================
	void AnimationDrawer::setBoundsFrameColor(const ColorF& color)
	{
		m_boundsFrameColor = color;
	}

	//================================================================================
	void AnimationDrawer::draw(const AnimationController* pAnimationController, double x, double y) const
	{
		if (pAnimationController == nullptr)
		{
			return;
		}
		const auto* pAnimSetting = pAnimationController->getAnimationSetting();
		if (pAnimSetting == nullptr)
		{
			return;
		}
		const auto& offset = pAnimSetting->canvasSize * Float2{ pAnimSetting->pivot.x + 0.5f, -pAnimSetting->pivot.y + 0.5f };
		drawPivot(pAnimationController, x + offset.x, y + offset.y);
	}

	//================================================================================
	void AnimationDrawer::draw(double x, double y) const
	{
		draw(m_pController, x, y);
	}

	//================================================================================
	void AnimationDrawer::draw(const Vec2& pos) const
	{
		draw(m_pController, pos.x, pos.y);
	}

	//================================================================================
	void AnimationDrawer::drawAt(const AnimationController* pAnimationController, double centerX, double centerY) const
	{
		if (pAnimationController == nullptr)
		{
			return;
		}
		const auto* pAnimSetting = pAnimationController->getAnimationSetting();
		if (pAnimSetting == nullptr)
		{
			return;
		}
		const auto& offset = pAnimSetting->canvasSize * Float2{ pAnimSetting->pivot.x, -pAnimSetting->pivot.y };
		drawPivot(pAnimationController, centerX + offset.x, centerY + offset.y);
	}

	//================================================================================
	void AnimationDrawer::drawAt(double centerX, double centerY) const
	{
		drawAt(m_pController, centerX, centerY);
	}

	//================================================================================
	void AnimationDrawer::drawAt(const Vec2& center) const
	{
		drawAt(m_pController, center.x, center.y);
	}

	//================================================================================
	void AnimationDrawer::drawPivot(const AnimationController* pAnimationController, double x, double y) const
	{
		if (pAnimationController == nullptr)
		{
			return;
		}
		const auto* pProjectSetting = pAnimationController->getProjectSetting();
		if (pProjectSetting == nullptr)
		{
			return;
		}
		for (const auto& pPartState : pAnimationController->getDrawPartStates())
		{
			if (pPartState == nullptr)
			{
				continue;
			}
			const auto& pModelPart = pPartState->pAnimationModelPart;
			if (pModelPart == nullptr)
			{
				continue;
			}

			const BlendState blend{ GetBlendState(pModelPart->blendType) };
			SamplerState sampler{ pProjectSetting->addressMode, pProjectSetting->filterMode };
			const Cellmap* pCellmap = pPartState->cellmapTextureInfo.pCellmap;
			if (pCellmap != nullptr and pCellmap->overrideSettingOpt.has_value())
			{
				const auto& overrideSetting = pCellmap->overrideSettingOpt.value();
				sampler = SamplerState{ overrideSetting.addressMode, overrideSetting.filterMode };
			}
			const ScopedRenderStates2D renderState{ blend, sampler };
			switch (pModelPart->partType)
			{
			case PartType::Normal:
			{
				const Transformer2D drawTrans{ Mat3x2::Translate(x, y) };
				drawPartNormal(pPartState);
				break;
			}
			case PartType::Instance:
			{
				drawPartInstance(pPartState, x, y);
				break;
			}
			case PartType::Mesh:
			{
				const Transformer2D drawTrans{ Mat3x2::Translate(x, y) };
				drawPartMesh(pPartState);
				break;
			}
			default:
				break;
			}

			drawPartBounds(pPartState);
		}
	}

	//================================================================================
	void AnimationDrawer::drawPivot(double x, double y) const
	{
		drawPivot(m_pController, x, y);
	}

	//================================================================================
	void AnimationDrawer::drawPivot(const Vec2& pos) const
	{
		drawPivot(m_pController, pos.x, pos.y);
	}

	//================================================================================
	void AnimationDrawer::drawPartNormal(const AnimationPartState* pPartState) const
	{
		if (pPartState == nullptr or pPartState->isHide)
		{
			return;
		}

		const auto* pModelPart = pPartState->pAnimationModelPart;
		if (pModelPart == nullptr)
		{
			return;
		}

		const Texture* pTexture = pPartState->cellmapTextureInfo.pTexture;
		const Cell* pCell = pPartState->cellmapTextureInfo.pCell;
		Buffer2D* pBuffer2D = pPartState->pBuffer2D.get();
		if (pTexture == nullptr
			or pCell == nullptr
			or pBuffer2D == nullptr)
		{
			return;
		}

		const auto& uvScale = pPartState->uvScale;
		if (uvScale.x == 0.0f or uvScale.y == 0.0f)
		{
			return;
		}

		// パーツカラー使用
		if (pPartState->usePartsColor)
		{
			const Float4 oldColors[] =
			{
				pBuffer2D->vertices[0].color, pBuffer2D->vertices[1].color,
				pBuffer2D->vertices[2].color, pBuffer2D->vertices[3].color,
			};
			const auto& partsColor = pPartState->partsColor;
			// 下地のアルファ値を設定
			for (int i = 0; i < 4; i++)
			{
				if (partsColor.blendType == BlendType::Mix)
				{
					break;
				}
				int32 colorIndex = 0;
				if (partsColor.target == ColorBlendTarget::Vertex)
				{
					colorIndex = i;
				}
				pBuffer2D->vertices[i].color.w *= (partsColor.colors[colorIndex].rgba.a / 255.0f);

				// Mulの場合、直接色にかけて表示。ブレンドだとアルファ値の扱いが違う？
				// サンプルと同じにするためにこうしているが、たぶん違う・・・
				if (partsColor.blendType == BlendType::Mul)
				{
					pBuffer2D->vertices[i].color *= ColorF{ partsColor.colors[colorIndex].rgba }.toFloat4();
				}
			}
			// 下地を描画
			{
				pBuffer2D->draw(*pTexture);

				// Mulの場合無理やり乗算描画のため下地だけで終わり。
				if (partsColor.blendType == BlendType::Mul)
				{
					// 加工した色を戻しておく
					for (int32 i = 0; i < 4; i++)
					{
						pBuffer2D->vertices[i].color = oldColors[i];
					}
					return;
				}
			}
			// ブレンド用色加工
			for (int32 i = 0; i < 4; i++)
			{
				int32 colorIndex = 0;
				if (partsColor.target == ColorBlendTarget::Vertex)
				{
					colorIndex = i;
				}
				ColorF blendColor{ partsColor.colors[colorIndex].rgba };
				if (partsColor.blendType == BlendType::Mix)
				{
					blendColor.a = static_cast<double>(partsColor.colors[colorIndex].rate);
				}
				pBuffer2D->vertices[i].color = blendColor.toFloat4();
				pBuffer2D->vertices[i].color.w *= oldColors[i].w; // 元のアルファ値をかける
			}
			// ブレンド
			{
				BlendState colorBlend{ GetBlendState(partsColor.blendType) };
				const ScopedRenderStates2D renderState{ colorBlend };
				pBuffer2D->draw();
			}
			// 加工した色を戻しておく
			for (int32 i = 0; i < 4; i++)
			{
				pBuffer2D->vertices[i].color = oldColors[i];
			}
			return;
		}

		// 通常描画
		pBuffer2D->draw(*pTexture);
	}

	//================================================================================
	void AnimationDrawer::drawPartInstance(const AnimationPartState* pPartState, double x, double y) const
	{
		if (pPartState == nullptr)
		{
			return;
		}

		const auto* pInstanceValue = std::get_if<AnimationPartStateInstance>(&pPartState->partValue);
		if (pInstanceValue == nullptr)
		{
			return;
		}
		const auto* pRefAnimationController = pInstanceValue->pRefAnimationController.get();
		drawPivot(pRefAnimationController, x, y);
	}

	//================================================================================
	void AnimationDrawer::drawPartMesh(const AnimationPartState* pPartState) const
	{
		if (pPartState == nullptr or pPartState->isHide)
		{
			return;
		}

		Buffer2D* pBuffer2D = pPartState->pBuffer2D.get();
		const Texture* pTexture = pPartState->cellmapTextureInfo.pTexture;
		if (pBuffer2D == nullptr
			or pTexture == nullptr)
		{
			return;
		}

		// パーツカラー使用
		if (pPartState->usePartsColor)
		{
			// メッシュのパーツカラーは単色のみ?
			const auto& partColor = pPartState->partsColor;
			ColorF blendColor{ partColor.colors[0].rgba };
			if (partColor.blendType == BlendType::Mix)
			{
				blendColor.a = static_cast<double>(partColor.colors[0].rate);
			}

			if (partColor.blendType == BlendType::Mul)
			{
				const ScopedColorMul2D colorMul{ blendColor };
				pBuffer2D->draw(*pTexture);
				return;
			}
			else
			{
				// 下地
				{
					pBuffer2D->draw(*pTexture);
				}

				// 色加工
				const float originalAlpha = pBuffer2D->vertices.begin()->color.w;
				for (auto& it : pBuffer2D->vertices)
				{
					it.color = blendColor.toFloat4();
				}

				// ブレンド描画
				{
					const ScopedRenderStates2D state{ GetBlendState(partColor.blendType) };
					pBuffer2D->draw();
				}

				// 加工した色を戻す
				for (auto& it : pBuffer2D->vertices)
				{
					it.color = Float4::One().withW(originalAlpha);
				}
				return;
			}
			return;
		}
		// 通常描画
		pBuffer2D->draw(*pTexture);
	}

	//================================================================================
	void AnimationDrawer::drawPartBounds(const AnimationPartState* pPartState) const
	{
		if (pPartState == nullptr)
		{
			return;
		}
		if (not(m_isBoundsDrawFill) and not(m_isBoundsDrawFrame))
		{
			return;
		}

		const auto* pBoundsVariant = pPartState->pBoundsValue.get();
		if (pBoundsVariant == nullptr)
		{
			return;
		}

		std::visit(
			[this](const auto& bounds)
			{
				if (m_isBoundsDrawFill)
				{
					bounds.draw(m_boundsColor);
				}
				if (m_isBoundsDrawFrame)
				{
					bounds.drawFrame(1.0, 1.0, m_boundsFrameColor);
				}
			}
			, *pBoundsVariant
		);
	}

	//================================================================================
	void AnimationDrawer::drawPartEffect(const AnimationPartState* pPartState) const
	{
		if (pPartState == nullptr)
		{
			return;
		}
		const auto* pEffectValue = std::get_if<AnimationPartStateEffect>(&pPartState->partValue);
		if (pEffectValue == nullptr)
		{
			return;
		}
		const auto& effectController = pEffectValue->effectController;
		const auto& emitters = effectController.getSortedEmitters();
		for (const auto& pEmitter : emitters)
		{
			if (pEmitter->pParent != nullptr)
			{
				const auto& particles = pEmitter->pParent->particles;
				for (const auto& particle : particles)
				{
					if (not(particle.isBorn))
					{
						continue;
					}
					const int32 frame = effectController.getTargetFrame() - static_cast<int32>(particle.startTime);
					drawEffectParticle(effectController, pEmitter, frame, pEmitter->pParent, &particle);
				}
			}
			else
			{
				drawEffectParticle(effectController, pEmitter, effectController.getTargetFrame(), nullptr, nullptr);
			}
		}
	}

	//================================================================================
	void AnimationDrawer::drawEffectParticle(const EffectController& controller, const EffectEmitter* pEmitter, int32 frame, const EffectEmitter* pParent, const EffectParticle* pParticle) const
	{
		if (pEmitter == nullptr)
		{
			return;
		}

		if (pEmitter->pCell == nullptr
			or pEmitter->pTexture == nullptr
			or pEmitter->pBehavior == nullptr)
		{
			return;
		}

		double parentAlpha = 1.0f;
		if (const auto* pParentState = controller.getParentState())
		{
			parentAlpha = pParentState->alpha;
		}

		const auto blendState = GetBlendState(pEmitter->pBehavior->blendType);
		const ScopedRenderStates2D state{ blendState };
		for (const auto& it : pEmitter->particleDrawInfos)
		{
			if (it.id < 0)
			{
				continue;
			}

			ColorF color{ it.color };
			color.a *= parentAlpha;
			if (0.0f < color.a)
			{
				continue;
			}
			Mat4x4 mat = Mat4x4::Identity();
			mat = mat * Mat4x4::Translate(Float3(it.pos * controller.getLayoutScale(), 0.0f));
			mat = mat * Mat4x4::RotateZ(ToRadians(it.rotation) + it.direction);
			mat = mat * Mat4x4::Scale(Float3(it.scale, 1.0f));
		}
	}

}
