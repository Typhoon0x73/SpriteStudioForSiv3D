#include "SSAnimationController.hpp"
#include "PartState/SSAnimationPartStateBuilder.hpp"
#include "Interpolation/SSInterpolationUtilities.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	AnimationController::AnimationController()
		: m_pProject{ nullptr }
		, m_pCurrentAnimationPack{ nullptr }
		, m_pCurrentAnimation{ nullptr }
		, m_pAnimationSetting{ nullptr }
		, m_frame{ 0 }
		, m_isNeedApply{ false }
		, m_partStates{}
		, m_sortPartStates{}
		, m_meshPartStates{}
		, m_bonePartStates{}
		, m_isFoundKeyHide{ false }
		, m_isFoundKeySizeX{ false }
		, m_isFoundKeySizeY{ false }
	{
	}

	//================================================================================
	AnimationController::AnimationController(const Project* pProject, StringView animationPackName, StringView animationName)
		: AnimationController{}
	{
		if (not(build(pProject, animationPackName, animationName)))
		{
			clear();
		}
	}

	//================================================================================
	AnimationController::~AnimationController()
	{
		clear();
	}

	//================================================================================
	void AnimationController::clear()
	{
		m_frame = 0;
		m_isNeedApply = false;
		m_bonePartStates.clear();
		m_meshPartStates.clear();
		m_sortPartStates.clear();
		m_partStates.clear();
		m_pAnimationSetting = nullptr;
		m_pCurrentAnimation = nullptr;
		m_pCurrentAnimationPack = nullptr;
		m_pProject = nullptr;
	}

	//================================================================================
	bool AnimationController::build(const Project* pProject, StringView animationPackName, StringView animationName)
	{
		if (pProject == nullptr)
		{
			return false;
		}

		const auto* pAnimationPack = pProject->findAnimationPack(animationPackName);
		if (pAnimationPack == nullptr)
		{
			return false;
		}

		const auto* pAnimation = pAnimationPack->findAnimation(animationName);
		if (pAnimationPack == nullptr)
		{
			return false;
		}

		// 一度クリアしてから作成開始。
		clear();

		// パーツを作成。
		if (not(createAnimationPartStates(pProject, pAnimationPack, pAnimation)))
		{
			return false;
		}

		// 親子付け
		for (auto& partState : m_partStates)
		{
			if (const auto* pModelPart = partState->pAnimationModelPart)
			{
				int32 parentIndex = pModelPart->parentIndex;
				if (parentIndex < 0)
				{
					continue;
				}
				partState->pParent = m_partStates[parentIndex].get();
			}
		}
		
		// メッシュパーツのバインド
		if (not(meshPartBind(pAnimationPack)))
		{
			// バインド失敗
			return false;
		}

		// アニメーション設定を取得しておく
		if (pAnimation->overrideSettingOpt.has_value())
		{
			m_pAnimationSetting = &(pAnimation->overrideSettingOpt.value());
		}
		else
		{
			m_pAnimationSetting = &(pAnimationPack->setting);
		}

		if (m_pAnimationSetting == nullptr)
		{
			// アニメーション設定が無いと、フレーム計算等できない。
			return false;
		}

		// 参照を保存する。
		m_pProject = pProject;
		m_pCurrentAnimationPack = pAnimationPack;
		m_pCurrentAnimation = pAnimation;

		// ソート
		partsSort();

		// 初期状態を更新しておく
		setFrame(m_pAnimationSetting->startFrame);
		apply();

		return true;
	}

	//================================================================================
	void AnimationController::apply()
	{
		if (not(isReady()) or not(m_isNeedApply))
		{
			return;
		}

		// 各パーツを更新
		for (auto& pPartState : m_partStates)
		{
			if (pPartState == nullptr)
			{
				continue;
			}
			updatePartState(pPartState.get(), m_frame);
		}

		// ソート
		partsSort();

		// 更新フラグを折って完了。
		m_isNeedApply = false;
	}

	//================================================================================
	void AnimationController::setFrame(int32 frame)
	{
		if (m_frame == frame)
		{
			return;
		}
		m_frame = frame;
		m_isNeedApply = true;
	}

	//================================================================================
	bool AnimationController::isReady() const noexcept
	{
		if (
			(m_pProject == nullptr)
			or (m_pCurrentAnimationPack == nullptr)
			or (m_pCurrentAnimation == nullptr)
			or (m_pAnimationSetting == nullptr)
			)
		{
			return false;
		}
		return true;
	}

	//================================================================================
	const ProjectSetting* const AnimationController::getProjectSetting() const noexcept
	{
		if (m_pProject == nullptr)
		{
			return nullptr;
		}
		return &(m_pProject->getProjectSetting());
	}

	//================================================================================
	Array<std::unique_ptr<AnimationPartState>>& AnimationController::getPartStatesRaw() noexcept
	{
		return m_partStates;
	}

	//================================================================================
	bool AnimationController::getFirstCellValue(const AnimationPart* pSetupPart, const AnimationPart* pAnimationPart, AttributeValueCell& out)
	{
		const AnimationAttribute* pCellAttribute = nullptr;
		if (pSetupPart != nullptr)
		{
			pCellAttribute = pSetupPart->findAttribute(AttributeKind::Cell);
		}
		// セットアップに無ければアニメーションから探す
		if (pCellAttribute == nullptr
			and pAnimationPart != nullptr)
		{
			pCellAttribute = pAnimationPart->findAttribute(AttributeKind::Cell);
		}
		if (pCellAttribute == nullptr)
		{
			// 参照セル情報がない。
			return false;
		}
		const auto* pCellKeyFrame = pCellAttribute->getFirstKey();
		if (pCellKeyFrame == nullptr)
		{
			// アトリビュートが空？？？
			return false;
		}
		if (not(std::holds_alternative<AttributeValueCell>(pCellKeyFrame->attributeVariantValue)))
		{
			// セル情報ではない？？？
			return false;
		}
		// データ取得。
		out = std::get<AttributeValueCell>(pCellKeyFrame->attributeVariantValue);
		return true;
	}

	//================================================================================
	bool AnimationController::getCellTexture(const AttributeValueCell& refCell, const Cell* pOutCell, const Texture* pOutTexture)
	{
		// プロジェクトデータがなければ設定できない。
		if (m_pProject == nullptr)
		{
			return false;
		}
		// セルマップから画像を探す
		const auto& cellmaps = m_pProject->getCellmaps();
		if (refCell.mapId < 0 or static_cast<int32>(cellmaps.size()) <= refCell.mapId)
		{
			// セルマップ配列の境界外。
			return false;
		}
		const auto& cellmapName = cellmaps[refCell.mapId].name;
		const auto& resourcePack = m_pProject->getResourcePack();
		const auto& cellmapTextureTable = resourcePack.cellmapTextureTable;
		const auto& cellmapTextureItr = cellmapTextureTable.find(cellmapName);
		if (cellmapTextureTable.end() == cellmapTextureItr)
		{
			// 画像データが見つからなかった。
			return false;
		}
		// セル、画像を設定する。
		pOutCell = m_pProject->findCell(refCell.mapId, refCell.name);
		pOutTexture = &(cellmapTextureItr->second);
		return true;
	}

	//================================================================================
	bool AnimationController::createAnimationPartStates(const Project* pProject, const AnimationPack* pAnimationPack, const Animation* pAnimation)
	{
		if (pAnimationPack == nullptr
			or pAnimation == nullptr)
		{
			return false;
		}

		const auto& model = pAnimationPack->model;
		const auto& pSetupAnimation = pAnimationPack->pSetupAnimation;

		// モデルパーツ分だけステートを用意
		const auto& modelParts = model.parts;
		const size_t modelPartsCount = modelParts.size();
		m_partStates.reserve(modelPartsCount);

		// モデルパーツと紐づいたアニメーションパーツを設定する
		for (size_t i = 0; i < modelPartsCount; i++)
		{
			const AnimationModelPart* pModelPart = &(modelParts[i]);
			const StringView partName = pModelPart->name;
			const AnimationPart* pSetupPart = pSetupAnimation->findAnimationPart(partName);
			const AnimationPart* pAnimationPart = pAnimation->findAnimationPart(partName);

			// セルを探す
			AttributeValueCell cellValue;
			Cell* pCell = nullptr;
			Texture* pTexture = nullptr;
			if (getFirstCellValue(pSetupPart, pAnimationPart, cellValue))
			{
				if (not(getCellTexture(cellValue, pCell, pTexture)))
				{
					// セル値見つけたけど、画像とセルが設定できない。
					return false;
				}
			}

			AnimationPartState* pStateRaw = AnimationPartStateBuilder::Build(pProject, pModelPart, pSetupPart, pAnimationPart, pCell, pTexture);
			if (pStateRaw == nullptr)
			{
				// パーツの作成に失敗。
				return false;
			}
			pStateRaw->index = static_cast<int32>(i);
			std::unique_ptr<AnimationPartState> pState = nullptr;
			pState.reset(pStateRaw);

			// メッシュパーツ
			if (pModelPart->partType == PartType::Mesh)
			{
				m_meshPartStates.emplace_back(pStateRaw);
			}
			// ボーンパーツ
			if (pModelPart->partType == PartType::Armature)
			{
				m_bonePartStates.emplace_back(pStateRaw);
			}

			// ソート用
			m_sortPartStates.emplace_back(pStateRaw);

			// パーツ登録
			m_partStates.emplace_back(std::move(pState));
		}

		return true;
	}

	//================================================================================
	void AnimationController::partsSort()
	{
		if (m_pAnimationSetting == nullptr)
		{
			// ソートの基準が分からない。
			return;
		}
		// 各ソートモードにしたがってソートする。
		switch (m_pAnimationSetting->sortMode)
		{
		case PartsSortMode::Prio:
		{
			m_sortPartStates.sort_by(
				[](const AnimationPartState* pLeft, const AnimationPartState* pRight)
				{
						if (pLeft->priority == pRight->priority)
						{
							return (pLeft->index < pRight->index);
						}
						return (pLeft->priority < pRight->priority);
				}
			);
			break;
		}
		case PartsSortMode::Z:
		{
			m_sortPartStates.sort_by(
				[](const AnimationPartState* pLeft, const AnimationPartState* pRight)
				{
					if (pLeft->position.z == pRight->position.z)
					{
						return (pLeft->index < pRight->index);
					}
					return (pLeft->position.z < pRight->position.z);
				}
			);
			break;
		}
		default:
			break;
		}
	}

	//================================================================================
	bool AnimationController::meshPartBind(const AnimationPack* pAnimationPack)
	{
		if (pAnimationPack == nullptr)
		{
			// アニメーションパック無しだとバインドできない。
			return false;
		}

		const auto& model = pAnimationPack->model;
		const auto& meshBinds = model.meshBinds;
		const size_t meshPartStateCount = m_meshPartStates.size();
		if (meshPartStateCount == 0)
		{
			// メッシュパーツ無し。
			return true;
		}

		if (meshPartStateCount != meshBinds.size())
		{
			// バインド数とパーツ数が一致しない。
			return false;
		}

		// インデックスとパーツの紐づけを行う。
		HashTable<int32, const AnimationPartState*> pBoneIndexTable;
		for (size_t i = 0; i < m_bonePartStates.size(); i++)
		{
			const auto& pBonePartState = m_bonePartStates[i];
			const auto* pModelPart = pBonePartState->pAnimationModelPart;
			if (pModelPart == nullptr)
			{
				// モデルパーツがないのはありえないが、一旦スルー
				continue;
			}
			pBoneIndexTable[static_cast<int32>(i)] = pBonePartState;
		}

		for (size_t i = 0; i < meshPartStateCount; i++)
		{
			const AnimationModelMeshBind& meshBind = meshBinds[i];
			if (not(std::holds_alternative<AnimationPartStateMesh>(m_meshPartStates[i]->partValue)))
			{
				// データが取れない。どこかで設定を間違えている。
				return false;
			}
			auto& meshPartStateValue = std::get<AnimationPartStateMesh>(m_meshPartStates[i]->partValue);
			auto& bindBoneInfoRaw = meshPartStateValue.bindBoneInfomations;
			const size_t meshVertexCount = meshPartStateValue.buffer2D.vertices.size();

			// モデルデータからメッシュパーツのパラメータへ反映する。
			const auto& modelBindInfomationsArray = meshBind.vertexBinds;
			for (size_t k = 0; k < modelBindInfomationsArray.size(); k++)
			{
				const auto& bindInfomations = modelBindInfomationsArray[k];

				if (meshVertexCount <= k)
				{
					// 頂点数超えたら設定できないので終わり。
					break;
				}

				int32 cntBone = 0;
				for (size_t n = 0; n < bindInfomations.size(); n++)
				{
					const auto& bindInfo = bindInfomations[n];
					auto& infoRaw = bindBoneInfoRaw[k].infomations[n];
					infoRaw.offset = bindInfo.offset;
					infoRaw.weight = bindInfo.weight;

					if (0 < pBoneIndexTable.count(bindInfo.boneIndex))
					{
						infoRaw.pState = pBoneIndexTable[bindInfo.boneIndex];
						cntBone++;
					}
				}
				bindBoneInfoRaw[k].bindBoneCount = cntBone;
			}
		}
		return true;
	}

	//================================================================================
	void AnimationController::initPartState(AnimationPartState* pPartState)
	{
	}

	//================================================================================
	void AnimationController::updatePartState(AnimationPartState* pPartState, int32 frame)
	{
		if (pPartState == nullptr)
		{
			// 更新したい対象がいないのは何事？？
			return;
		}

		// 各パーツの存在確認
		const auto* pParent = pPartState->pParent;
		const auto* pModelPart = pPartState->pAnimationModelPart;
		const auto* pSetupPart = pPartState->pSetupAnimationPart;
		const auto* pAnimationPart = pPartState->pAnimationPart;
		const bool existParent = (pParent != nullptr);
		const bool existSetupPart = (pSetupPart != nullptr);
		const bool existAnimationPart = (pAnimationPart != nullptr);
		const bool existModelPart = (pModelPart != nullptr);

		// セットアップもアニメーションも無ければ表示しないフラグを立てる。
		if (not(existSetupPart) and not(existAnimationPart))
		{
			pPartState->isHide = true;
		}

		// 初期化しておく
		initPartState(pPartState);

		m_isFoundKeyHide = false;
		m_isFoundKeySizeX = false;
		m_isFoundKeySizeY = false;


		// ボーンパーツであればボーン値で初期化する。
		if (existModelPart
			and std::holds_alternative<ModelPartInfoArmature>(pModelPart->partVariantValue))
		{
			const auto& boneValue = std::get<ModelPartInfoArmature>(pModelPart->partVariantValue);
			const auto& bonePos = boneValue.bonePosition;
			pPartState->position.x = bonePos.x;
			pPartState->position.y = bonePos.y;
			pPartState->rotation.z = boneValue.boneRotation;
		}

		// セットアップパーツがあればセットアップパーツで初期化する。
		if (existSetupPart)
		{
			updatePartStateAttributes(pPartState, pSetupPart->attributes, frame);
		}
		// アニメーションパーツがあればアニメーション値を適応する。
		if (existAnimationPart)
		{
			updatePartStateAttributes(pPartState, pAnimationPart->attributes, frame);
		}
	}

	//================================================================================
	void AnimationController::updatePartStateAttributes(AnimationPartState* pPartState, const Array<AnimationAttribute>& attributes, int32 frame)
	{
		if (pPartState == nullptr)
		{
			// 更新したい対象がいなければ何もできない。
			return;
		}

		// ローカル変数のほうがアクセスが早いのでキャッシュ
		int32 currentFrame = frame;

		// キーが見つからなかった時にセットアップパーツの情報が欲しいのでキャッシュしておく。
		const auto* pSetupPart = pPartState->pSetupAnimationPart;

		for (const auto& attribute : attributes)
		{
			switch (attribute.kind)
			{
			case AttributeKind::Cell:
			{
				AttributeValueCell prev = pPartState->refCell;
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->refCell);
				if (prev != pPartState->refCell and m_pProject != nullptr)
				{
					if (not(getCellTexture(pPartState->refCell, pPartState->pCell, pPartState->pTexture)))
					{
						// 画像、セルの設定に失敗している。
						pPartState->pCell = nullptr;
						pPartState->pTexture = nullptr;
					}
				}
				break;
			}
			case AttributeKind::PosX:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->position.x);
				break;
			}
			case AttributeKind::PosY:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->position.y);
				break;
			}
			case AttributeKind::PosZ:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->position.z);
				break;
			}
			case AttributeKind::RotateX:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->rotation.x);
				break;
			}
			case AttributeKind::RotateY:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->rotation.y);
				break;
			}
			case AttributeKind::RotateZ:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->rotation.z);
				break;
			}
			case AttributeKind::ScaleX:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->scale.x);
				break;
			}
			case AttributeKind::ScaleY:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->scale.y);
				break;
			}
			case AttributeKind::LocalScaleX:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->localScale.x);
				break;
			}
			case AttributeKind::LocalScaleY:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->localScale.y);
				break;
			}
			case AttributeKind::Alpha:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->alpha);
				break;
			}
			case AttributeKind::LocalAlpha:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->localAlpha);
				break;
			}
			case AttributeKind::Priority:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->priority);
				break;
			}
			case AttributeKind::Hide:
			{
				int32 useFrame = Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->isHide);
				// 非表示キーがないか、先頭の非表示キーより手前の場合は常に非表示にする。
				// セットアップによってm_isFoundHideKeyがあった場合は強制非表示にしない
				if (currentFrame < useFrame and not(m_isFoundKeyHide))
				{
					pPartState->isHide = true;
				}
				else
				{
					// 非表示キーがあり、かつ最初のキーフレームを取得した
					m_isFoundKeyHide = true;
				}
				break;
			}
			case AttributeKind::PartsColor:
			case AttributeKind::Shader:
			case AttributeKind::Vertex:
				break;
			case AttributeKind::PivotX:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->pivotOffset.x);
				break;
			}
			case AttributeKind::PivotY:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->pivotOffset.y);
				break;
			}
			case AttributeKind::SizeX:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->size.x);
				m_isFoundKeySizeX = true;
				break;
			}
			case AttributeKind::SizeY:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->size.y);
				m_isFoundKeySizeY = true;
				break;
			}
			case AttributeKind::ImageFlipH:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->isImageFlipHorizontal);
				break;
			}
			case AttributeKind::ImageFlipV:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->isImageFlipVertical);
				break;
			}
			case AttributeKind::UVTranslateX:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->uvTranslate.x);
				break;
			}
			case AttributeKind::UVTranslateY:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->uvTranslate.y);
				break;
			}
			case AttributeKind::UVRotateZ:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->uvRotation);
				break;
			}
			case AttributeKind::UVScaleX:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->uvScale.x);
				break;
			}
			case AttributeKind::UVScaleY:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->uvScale.y);
				break;
			}
			case AttributeKind::BoundRadius:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->boundingRadius);
				break;
			}
			case AttributeKind::Mask:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->maskThreshold);
				break;
			}
			case AttributeKind::User:
			case AttributeKind::Signal:
			case AttributeKind::InstanceParam:
			case AttributeKind::Effect:
			case AttributeKind::Deform:
			case AttributeKind::Audio:
			case AttributeKind::TextureChange:
				break;

				// 非対応
			case AttributeKind::FlipH:
			case AttributeKind::FlipV:
			case AttributeKind::VertexColor:
			case AttributeKind::AnchorX:
			case AttributeKind::AnchorY:
				// 無効値
			case AttributeKind::Invalid:
			default:
				break;
			}
		}
	}

}
