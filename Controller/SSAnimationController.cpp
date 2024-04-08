#include "SSAnimationController.hpp"
#include "PartState/SSAnimationPartStateBuilder.hpp"
#include "Interpolation/SSInterpolationUtilities.hpp"
#include "../Common/SSOutputDebugLog.hpp"

namespace s3d::SpriteStudio
{

	//================================================================================
	int32 AnimationController::FindAnimationLabelToFrame(const Animation* pAnimation, StringView label)
	{
		for (const auto& it : pAnimation->labels)
		{
			if (label == it.name)
			{
				return it.frame;
			}
		}
		return -1;
	}

	//================================================================================
	int32 AnimationController::AnimationLabelToFrame(StringView label, int32 offset, const Animation* pAnimation, const AnimationSetting* pSetting)
	{
		if (pSetting == nullptr)
		{
			return 0;
		}

		const int32 startFrame = pSetting->startFrame;
		const int32 endFrame = pSetting->endFrame;
		int32 ret = offset;

		if (label == U"_start")
		{
			ret = startFrame + offset;
		}
		else if (label == U"_end")
		{
			ret = endFrame + offset;
		}
		else if (label == U"none")
		{
			return ret;
		}
		else
		{
			int32 findFrame = FindAnimationLabelToFrame(pAnimation, label);

			if (findFrame < 0)
			{
				return ret;
			}
			ret = Clamp(findFrame + offset, startFrame, endFrame);
			return ret;
		}

		return Clamp(ret, startFrame, endFrame);
	}

	//================================================================================
	AnimationController::AnimationController()
		: m_pProject{ nullptr }
		, m_pCurrentAnimationPack{ nullptr }
		, m_pCurrentAnimation{ nullptr }
		, m_pAnimationSetting{ nullptr }
		, m_frame{ 0 }
		, m_prevFrame{ 0 }
		, m_isNeedApply{ false }
		, m_partStates{}
		, m_sortPartStates{}
		, m_meshPartStates{}
		, m_bonePartStates{}
		, m_isFoundKeyHide{ false }
		, m_isFoundKeySizeX{ false }
		, m_isFoundKeySizeY{ false }
		, m_effectSeedOffset{ 0 }
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
			DebugLog::Print(DebugLog::LogType::Error, U"プロジェクトデータを指定してください。");
			return false;
		}

		const auto* pAnimationPack = pProject->findAnimationPack(animationPackName);
		if (pAnimationPack == nullptr)
		{
			DebugLog::Print(DebugLog::LogType::Error, U"アニメーションパックが見つかりませんでした。:{}"_fmt(animationPackName));
			return false;
		}

		const auto* pAnimation = pAnimationPack->findAnimation(animationName);
		if (pAnimationPack == nullptr)
		{
			DebugLog::Print(DebugLog::LogType::Error, U"アニメーションが見つかりませんでした。:{}"_fmt(animationName));
			return false;
		}

		// 一度クリアしてから作成開始。
		clear();

		// パーツを作成。
		if (not(createAnimationPartStates(pProject, pAnimationPack, pAnimation)))
		{
			DebugLog::Print(DebugLog::LogType::Error, U"パーツの作成に失敗しました。");
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
			DebugLog::Print(DebugLog::LogType::Error, U"パーツのバインドに失敗しました。");
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
			DebugLog::Print(DebugLog::LogType::Error, U"アニメーション設定を取得できませんでした。");
			return false;
		}

		// 参照を保存する。
		m_pProject = pProject;
		m_pCurrentAnimationPack = pAnimationPack;
		m_pCurrentAnimation = pAnimation;

		// ソート
		partsSort();

		// 初期状態を更新しておく
		m_isNeedApply = true;
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
		m_prevFrame = m_frame;
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
	Size AnimationController::getCanvasSize() const noexcept
	{
		if (m_pAnimationSetting == nullptr)
		{
			return Size::Zero();
		}
		return m_pAnimationSetting->canvasSize;
	}

	//================================================================================
	int32 AnimationController::getFPS() const noexcept
	{
		if (m_pAnimationSetting == nullptr)
		{
			return 0;
		}
		return m_pAnimationSetting->fps;
	}

	//================================================================================
	int32 AnimationController::getFrameCount() const noexcept
	{
		if (m_pAnimationSetting == nullptr)
		{
			return 0;
		}
		return m_pAnimationSetting->frameCount;
	}

	//================================================================================
	int32 AnimationController::getStartFrame() const noexcept
	{
		if (m_pAnimationSetting == nullptr)
		{
			return -1;
		}
		return m_pAnimationSetting->startFrame;
	}

	//================================================================================
	int32 AnimationController::getEndFrame() const noexcept
	{
		if (m_pAnimationSetting == nullptr)
		{
			return -1;
		}
		return m_pAnimationSetting->endFrame;
	}

	//================================================================================
	Array<std::unique_ptr<AnimationPartState>>& AnimationController::getPartStatesRaw() noexcept
	{
		return m_partStates;
	}

	//================================================================================
	const Array<std::unique_ptr<AnimationPartState>>& AnimationController::getPartStates() const noexcept
	{
		return m_partStates;
	}

	//================================================================================
	const Array<AnimationPartState*>& AnimationController::getDrawPartStates() const noexcept
	{
		return m_sortPartStates;
	}

	//================================================================================
	const Project* const AnimationController::getProject() const noexcept
	{
		return m_pProject;
	}

	//================================================================================
	const AnimationPack* const AnimationController::getCurrentAnimationPack() const noexcept
	{
		return m_pCurrentAnimationPack;
	}

	//================================================================================
	const Animation* const AnimationController::getCurrentAnimation() const noexcept
	{
		return m_pCurrentAnimation;
	}

	//================================================================================
	const AnimationSetting* const AnimationController::getAnimationSetting() const noexcept
	{
		return m_pAnimationSetting;
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
			DebugLog::Print(DebugLog::LogType::Error, U"セルアトリビュートはありますが、キー値がありません。");
			return false;
		}
		if (not(std::holds_alternative<AttributeValueCell>(pCellKeyFrame->attributeVariantValue)))
		{
			// セル情報ではない？？？
			DebugLog::Print(DebugLog::LogType::Error, U"セルアトリビュート値がセル値として保存されていません。");
			return false;
		}
		// データ取得。
		out = std::get<AttributeValueCell>(pCellKeyFrame->attributeVariantValue);
		return true;
	}

	//================================================================================
	bool AnimationController::getCellTexture(const Project* pProject, const AttributeValueCell& refCell, CellmapTextureInfo& pOut)
	{
		// プロジェクトデータがなければ設定できない。
		if (pProject == nullptr)
		{
			DebugLog::Print(DebugLog::LogType::Error, U"プロジェクトデータを指定してください。");
			return false;
		}
		// セルマップから画像を探す
		const auto& cellmaps = pProject->getCellmaps();
		if (refCell.mapId < 0 or static_cast<int32>(cellmaps.size()) <= refCell.mapId)
		{
			// セルマップ配列の境界外。
			DebugLog::Print(DebugLog::LogType::Error, U"セルマップ配列の範囲外を指定しています。");
			return false;
		}
		const auto& cellmapName = cellmaps[refCell.mapId].name;
		const auto& resourcePack = pProject->getResourcePack();
		const auto& cellmapTextureTable = resourcePack.cellmapTextureTable;
		const auto& cellmapTextureItr = cellmapTextureTable.find(cellmapName);
		if (cellmapTextureTable.end() == cellmapTextureItr)
		{
			// 画像データが見つからなかった。
			DebugLog::Print(DebugLog::LogType::Error, U"画像データが見つかりませんでした。cellmapName:{}"_fmt(cellmapName));
			return false;
		}
		// セル、画像を設定する。
		pOut.pCellmap = pProject->getCellmap(refCell.mapId);
		pOut.pCell = pProject->findCell(refCell.mapId, refCell.name);
		pOut.pTexture = &(cellmapTextureItr->second);
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
		const auto& pSetupAnimation = pAnimationPack->getSetupAnimation();

		// モデルパーツ分だけステートを用意
		const auto& modelParts = model.parts;
		const size_t modelPartsCount = modelParts.size();
		m_partStates.reserve(modelPartsCount);

		// モデルパーツと紐づいたアニメーションパーツを設定する
		for (size_t i = 0; i < modelPartsCount; i++)
		{
			const AnimationModelPart* pModelPart = &(modelParts[i]);
			const StringView partName = pModelPart->name;
			const AnimationPart* pSetupPart = ((pSetupAnimation != nullptr) ? pSetupAnimation->findAnimationPart(partName) : nullptr);
			const AnimationPart* pAnimationPart = ((pAnimation != nullptr) ? pAnimation->findAnimationPart(partName) : nullptr);

			// セルを探す
			AttributeValueCell cellValue;
			CellmapTextureInfo cellmapTextureInfo;
			if (getFirstCellValue(pSetupPart, pAnimationPart, cellValue))
			{
				if (not(getCellTexture(pProject, cellValue, cellmapTextureInfo)))
				{
					// セル値見つけたけど、画像とセルが設定できない。
					return false;
				}
			}

			AnimationPartState* pStateRaw = AnimationPartStateBuilder::Build(pProject, pModelPart, pSetupPart, pAnimationPart, cellmapTextureInfo);
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
			DebugLog::Print(DebugLog::LogType::Error, U"アニメーションパック参照がありませんでした。");
			return false;
		}

		const auto& model = pAnimationPack->model;
		const size_t meshPartStateCount = m_meshPartStates.size();
		if (meshPartStateCount == 0)
		{
			// メッシュパーツ無し。
			return true;
		}

		const auto& modelMeshBinds = model.meshBinds;
		if (meshPartStateCount != modelMeshBinds.size())
		{
			// バインド数とパーツ数が一致しない。
			DebugLog::Print(DebugLog::LogType::Error, U"バインド数とパーツ数が一致していません。");
			return false;
		}

		// インデックスとパーツの紐づけを行う。
		const auto& boneTable = model.boneTable;
		HashTable<int32, const AnimationPartState*> pBoneIndexTable;
		for (const auto& pBoneState : m_bonePartStates)
		{
			const auto* pModelPart = pBoneState->pAnimationModelPart;
			if (pModelPart == nullptr)
			{
				continue;
			}
			auto bone = boneTable.find(pModelPart->name);
			if (bone == boneTable.end())
			{
				continue;
			}
			const int32 idx = bone->second;
			pBoneIndexTable[idx] = pBoneState;
		}

		for (size_t i = 0; i < meshPartStateCount; i++)
		{
			const AnimationModelMeshBind& modelMeshBind = modelMeshBinds[i];
			AnimationPartStateMesh* pMeshState = std::get_if<AnimationPartStateMesh>(&(m_meshPartStates[i]->partValue));
			auto& bindBoneInfoRaw = pMeshState->bindBoneInfomations;

			const auto& modelBindInfomationsArray = modelMeshBind.vertexBinds;
			size_t meshVertexCount = 0;
			if (const auto* pBuffer = m_meshPartStates[i]->pBuffer2D.get())
			{
				meshVertexCount = pBuffer->vertices.size();
			}
			for (size_t k = 0; k < modelBindInfomationsArray.size(); k++)
			{
				if (meshVertexCount <= k)
				{
					continue;
				}
				const auto& modelBindInfomations = modelBindInfomationsArray[k];
				int32 cntBone = 0;
				for (int32 n = 0; n < modelMeshBind.bindBoneNum; n++)
				{
					const auto& modelBindInfo = modelBindInfomations[n];
					auto& infoRaw = bindBoneInfoRaw[k].infomations[n];
					infoRaw.offset = modelBindInfo.offset;
					infoRaw.weight = modelBindInfo.weight;

					if (0 < pBoneIndexTable.count(modelBindInfo.boneIndex))
					{
						infoRaw.pState = pBoneIndexTable[modelBindInfo.boneIndex];
						cntBone++;
					}
				}
				bindBoneInfoRaw[k].bindBoneCount = cntBone;
			}
		}
		return true;
	}

	//================================================================================
	void AnimationController::initState(AnimationPartState* pPartState)
	{
		pPartState->modelMatrix = Mat4x4::Identity();
		pPartState->localModelMatrix = Mat4x4::Identity();

		pPartState->position = Float3::Zero();
		pPartState->rotation = Float3::Zero();
		pPartState->scale = Float2::One();
		pPartState->localScale = Float2::One();

		pPartState->alpha = 1.0f;
		pPartState->localAlpha = 1.0f;

		pPartState->priority = 0;
		pPartState->isHide = false;

		pPartState->pivotOffset = Float2::One();
		pPartState->size = Float2::One();

		pPartState->isImageFlipHorizontal = false;
		pPartState->isImageFlipVertical = false;

		pPartState->uvTranslate = Float2::Zero();
		pPartState->uvRotation = 0.0f;
		pPartState->uvScale = Float2::One();

		pPartState->boundingRadius = 0.0f;
		pPartState->maskThreshold = 0;

		pPartState->usePartsColor = false;
		pPartState->useShader = false;
		pPartState->useVertexTrans = false;
		pPartState->useLocalAlpha = false;
		pPartState->useLocalScale = false;
		pPartState->useDeform = false;

		auto& effectParamRaw = pPartState->effectParam;
		effectParamRaw.isIndependent = false;
		effectParamRaw.speed = 1.0f;
		effectParamRaw.startTime = 0;
		effectParamRaw.currentKeyFrame = 0;
		pPartState->effectSeed = 0;
		pPartState->effectTime = 0;

		auto& instanceParamRaw = pPartState->instanceParam;
		instanceParamRaw.isInfinity = false;
		instanceParamRaw.isReverse = false;
		instanceParamRaw.isPingPong = false;
		instanceParamRaw.isIndependent = false;
		instanceParamRaw.loopNum = 1;
		instanceParamRaw.startLabel = U"_start";
		instanceParamRaw.startOffset = 0;
		instanceParamRaw.endLabel = U"_end";
		instanceParamRaw.endOffset = 0;
		instanceParamRaw.curKeyframe = 0;
		instanceParamRaw.speed = 1.0f;
		instanceParamRaw.startFrame = 0;
		instanceParamRaw.endFrame = 0;

		m_isFoundKeyHide = false;
		m_isFoundKeySizeX = false;
		m_isFoundKeySizeY = false;
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

		// 初期化しておく
		initState(pPartState);

		// シェイプのサイズを設定
		if (existModelPart and pModelPart->partType == PartType::Shape)
		{
			pPartState->size = Float2{ 64.0f, 64.0f };
		}

		// セットアップもアニメーションも無ければ表示しないフラグを立てておわる。
		if (not(existSetupPart) and not(existAnimationPart))
		{
			pPartState->isHide = true;
			return;
		}

		// 親の継承設定を引用する設定の場合、ここで親のものに変えておく。
		if (existModelPart and pModelPart->inheritType == InheritType::Parent)
		{
			if (existParent)
			{
				pPartState->alphaInheritRate = pParent->alphaInheritRate;
			}
		}

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

		// マトリクスの更新。
		pPartState->updateMatrix();

		// 非表示キーがないか、先頭の非表示キーより手前の場合は常に非表示にする。
		if (not(m_isFoundKeyHide))
		{
			pPartState->isHide = true;
		}

		// 継承
		{
			if (existParent)
			{
				const float alphaInheritRate = pPartState->alphaInheritRate;
				if (0.0f < alphaInheritRate)
				{
					pPartState->alpha *= (pParent->alpha * alphaInheritRate);
				}

				// 親がインスタンスパーツでかつ非表示フラグがある場合は非表示にする。
				if (m_isInstancePartsHide)
				{
					pPartState->isHide = true;
				}
			}
		}

		if (existModelPart)
		{
			const auto& partType = pModelPart->partType;

			// 頂点の設定
			if (partType == PartType::Normal
				or partType == PartType::Mask
				or partType == PartType::Text
				)
			{
				if (pPartState->cellmapTextureInfo.pCell != nullptr
					and (existAnimationPart or existSetupPart))
				{
					if (not(m_isFoundKeySizeX))
					{
						pPartState->size.x = static_cast<float>(pPartState->cellmapTextureInfo.pCell->rect.size.x);
					}
					if (not(m_isFoundKeySizeY))
					{
						pPartState->size.y = static_cast<float>(pPartState->cellmapTextureInfo.pCell->rect.size.y);
					}
				}
				pPartState->updateVertices();
			}

			if (partType == PartType::Nineslice
				or partType == PartType::Shape
				)
			{
				pPartState->updateVertices();
			}

			// インスタンスパーツの更新
			if (partType == PartType::Instance)
			{
				updateInstance(pPartState, frame);
				pPartState->updateVertices();
			}

			// エフェクトパーツの更新
			if (partType == PartType::Effect)
			{
				updateEffect(pPartState, frame);
			}

			// メッシュパーツの更新
			if (partType == PartType::Mesh)
			{
				updateMesh(pPartState);
			}
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

		// ローカル変数にキャッシュ
		const int32 currentFrame = frame;

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
				if (m_pProject != nullptr and prev != pPartState->refCell)
				{
					if (not(getCellTexture(m_pProject, pPartState->refCell, pPartState->cellmapTextureInfo)))
					{
						// 画像、セルの設定に失敗している。
						pPartState->cellmapTextureInfo = CellmapTextureInfo{};
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
				pPartState->useLocalScale = true;
				break;
			}
			case AttributeKind::LocalScaleY:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->localScale.y);
				pPartState->useLocalScale = true;
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
				pPartState->useLocalAlpha = true;
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
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->partsColor);
				pPartState->usePartsColor = true;
				break;
			}
			case AttributeKind::Shader:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->shaderParam);
				pPartState->useShader = true;
				break;
			}
			case AttributeKind::Vertex:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->vertexTrans);
				pPartState->useVertexTrans = true;
				break;
			}
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
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->userParam);
				break;
			}
			case AttributeKind::Signal:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->signalParam);
				break;
			}
			case AttributeKind::InstanceParam:
			{
				int32 useFrame = Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->instanceParam);
				// 先頭にキーがない場合は初期値を入れておく。
				if (currentFrame < useFrame)
				{
					pPartState->instanceParam = AttributeValueInstance();
				}
				break;
			}
			case AttributeKind::Effect:
			{
				int32 useFrame = Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->effectParam);

				// 先頭にキーがない場合は初期値を入れておく。
				if (currentFrame < useFrame)
				{
					pPartState->effectParam = AttributeValueEffect();
				}
				else
				{
					pPartState->effectTime = useFrame;
					// 初回のみ初期化
					if (not(pPartState->effectParam.isAttributeInitialized))
					{
						pPartState->effectParam.isAttributeInitialized = true;
						pPartState->effectTimeTotal = static_cast<float>(pPartState->effectParam.startTime);
					}
				}
				break;
			}
			case AttributeKind::Deform:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->deformParam);
				pPartState->useDeform = true;
				break;
			}
			case AttributeKind::Audio:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->audioParam);
				break;
			}
			case AttributeKind::TextureChange:
			{
				Utilities::GetKeyValue(currentFrame, pSetupPart, attribute, pPartState->textureChangeParam);
				break;
			}

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

	//================================================================================
	void AnimationController::updateInstance(AnimationPartState* pPartState, int32 frame)
	{
		if (pPartState == nullptr)
		{
			return;
		}

		auto* pInstanceValue = std::get_if<AnimationPartStateInstance>(&(pPartState->partValue));
		if (pInstanceValue == nullptr)
		{
			// インスタンス値を持っていない？
			return;
		}
		auto* pRefAnimationController = pInstanceValue->pRefAnimationController.get();
		if (pRefAnimationController == nullptr)
		{
			// 参照アニメーションなし？？
			return;
		}

		auto& instanceParamRaw = pPartState->instanceParam;
		const auto* pRefAnimation = pRefAnimationController->getCurrentAnimation();
		const auto* pRefAnimationSetting = pRefAnimationController->getAnimationSetting();
		const int32 startFrame = AnimationLabelToFrame(instanceParamRaw.startLabel, instanceParamRaw.startOffset, pRefAnimation, pRefAnimationSetting);
		const int32 endFrame = AnimationLabelToFrame(instanceParamRaw.endLabel, instanceParamRaw.endOffset, pRefAnimation, pRefAnimationSetting);
		instanceParamRaw.startFrame = startFrame;
		instanceParamRaw.endFrame = endFrame;

		int32 playFrame = frame;

		// 独立動作の場合
		if (instanceParamRaw.isIndependent)
		{
			const int32 delta = frame - m_prevFrame;
			instanceParamRaw.liveFrame += (delta * instanceParamRaw.speed);
			playFrame = static_cast<int32>(instanceParamRaw.liveFrame);
		}

		// このインスタンスが配置されたキーフレーム（絶対時間）
		const int32 selfTopKeyFrame = instanceParamRaw.curKeyframe;


		int32 refTime = static_cast<int32>((playFrame - selfTopKeyFrame) * instanceParamRaw.speed);
		if (refTime < 0)
		{
			return; // そもそも生存時間に存在していない
		}
		if (playFrame < selfTopKeyFrame)
		{
			return;
		}
		int32 instanceScale = (endFrame - startFrame) + 1; // インスタンスの尺

		// 尺が０もしくはマイナス（あり得ない
		if (instanceScale <= 0)
		{
			return;
		}

		int32 nowLoop = (refTime / instanceScale); // 現在までのループ数

		int32 checkLoopNum = instanceParamRaw.loopNum;

		// pingpongの場合では２倍にする
		if (instanceParamRaw.isPingPong)
		{
			checkLoopNum = checkLoopNum * 2;
		}

		// 無限ループで無い時にループ数をチェック
		if (not(instanceParamRaw.isInfinity)) // 無限フラグが有効な場合はチェックせず
		{
			if (checkLoopNum <= nowLoop)
			{
				refTime = instanceScale - 1;
				nowLoop = checkLoopNum - 1;
			}
		}

		int32 tempFrame = refTime % instanceScale; // ループを加味しないインスタンスアニメ内のフレーム

		//参照位置を決める
		//現在の再生フレームの計算
		int32 applyFrame = 0;
		bool isReverse = instanceParamRaw.isReverse;
		if (instanceParamRaw.isPingPong and (nowLoop % 2 == 1))
		{
			isReverse = !isReverse; // 反転
		}

		if (isReverse)
		{
			// リバースの時
			applyFrame = endFrame - tempFrame;
		}
		else
		{
			// 通常時
			applyFrame = tempFrame + startFrame;
		}

		// ローカル不透明度を使用する場合、継承の関係で値をローカル値にしておく。
		const float originalAlpha = pPartState->alpha;
		if (pPartState->useLocalAlpha)
		{
			pPartState->alpha = pPartState->localAlpha; // ローカル値適応
		}

		pRefAnimationController->m_isInstancePartsHide = pPartState->isHide;
		pRefAnimationController->setFrame(applyFrame);
		pRefAnimationController->apply();

		// 戻す
		pPartState->alpha = originalAlpha;
	}

	//================================================================================
	void AnimationController::updateEffect(AnimationPartState* pPartState, int32 frame)
	{
		if (pPartState == nullptr
			or pPartState->isHide)
		{
			return;
		}

		auto* pEffectPartState = std::get_if<AnimationPartStateEffect>(&(pPartState->partValue));
		if (pEffectPartState == nullptr)
		{
			return;
		}
		auto& refEffectControllerRaw = pEffectPartState->effectController;

		// 独立動作
		if (pPartState->effectParam.isIndependent)
		{
			// 初期化済み
			if (pPartState->effectParam.isAttributeInitialized)
			{
				pPartState->effectTimeTotal += (frame - m_prevFrame) * pPartState->effectParam.speed;
				refEffectControllerRaw.setLoop(LoopEnable::Yes);
				refEffectControllerRaw.setFrame(static_cast<int32>(pPartState->effectTimeTotal));
				refEffectControllerRaw.play();
				refEffectControllerRaw.apply();
			}
		}
		else
		{
			float playTime = (float)(frame - pPartState->effectTime);
			if (playTime < 0)
			{
				return;
			}

			playTime *= pPartState->effectParam.speed;
			playTime += pPartState->effectParam.startTime;

			refEffectControllerRaw.setSeedOffset(m_effectSeedOffset);
			refEffectControllerRaw.setFrame(static_cast<int32>(playTime));
			refEffectControllerRaw.play();
			refEffectControllerRaw.apply();
		}
	}

	//================================================================================
	void AnimationController::updateMesh(AnimationPartState* pPartState)
	{
		if (pPartState == nullptr)
		{
			return;
		}
		auto* pBufferRaw = pPartState->pBuffer2D.get();
		auto* pMeshStateValue = std::get_if<AnimationPartStateMesh>(&(pPartState->partValue));
		if (pBufferRaw == nullptr
			or pMeshStateValue == nullptr)
		{
			return;
		}
		const size_t vertexCount = pBufferRaw->vertices.size();
		const auto& matrix = pPartState->modelMatrix;
		const auto& worldMatrix = pPartState->getWorldMatrix();
		for (int32 i = 0; i < vertexCount; i++)
		{
			// デフォームを利用している場合、一旦計算用に保存
			if (pPartState->useDeform)
			{
				Float2 offset = Float2::Zero();
				const auto& deformParam = pPartState->deformParam;
				const size_t deformParamCount = deformParam.verticesChanges.size();
				if (vertexCount == deformParamCount)
				{
					// ワールド座標を取得
					offset = pMeshStateValue->vertexPositions[i] + deformParam.verticesChanges[i];
					offset = matrix.transformPoint(Float3{ offset, 0.0f }).xy();
					Float2 tmp = matrix.transformPoint(Float3{ pMeshStateValue->vertexPositions[i], 0.0f }).xy();
					offset = offset - tmp;
				}
				pMeshStateValue->worldVertexPositions[i] = offset;
			}

			const int32 bindBoneCount = pMeshStateValue->bindBoneInfomations[i].bindBoneCount;
			if (0 == bindBoneCount)
			{
				// バインドされていないメッシュの場合
				// デフォームオフセットを加える
				const Float2 offset = pMeshStateValue->vertexPositions[i] + calcOffsetLocalVertexPos(pPartState, i);
				const Float2 pos = worldMatrix.transformPoint(Float3{ offset, 0.0f }).xy();
				pBufferRaw->vertices[i].pos.x = pos.x;
				pBufferRaw->vertices[i].pos.y = -pos.y;
			}
			else
			{
				Float2 offsetTotal = Float2::Zero();
				// ボーンの影響計算
				for (int32 n = 0; n < bindBoneCount; n++)
				{
					const auto& info = pMeshStateValue->bindBoneInfomations[i].infomations[n];
					const auto* pState = info.pState;
					if (pState == nullptr)
					{
						continue;
					}
					const float weight = static_cast<float>(info.weight) / 100.0f;
					const auto& boneMat = pState->modelMatrix;
					Float2 boneOffset = boneMat.transformPoint(Float3{ info.offset, 0.0f }).xy();
					boneOffset *= weight;
					offsetTotal += boneOffset;
				}
				const auto& invMat = matrix.inverse();
				const Float2 tmp = invMat.transformPoint(Float3{ offsetTotal, 0.0f }).xy();
				offsetTotal = tmp + calcOffsetLocalVertexPos(pPartState, i);
				const Float2 pos = matrix.transformPoint(Float3{ offsetTotal, 0.0f }).xy();
				pBufferRaw->vertices[i].pos.x = pos.x;
				pBufferRaw->vertices[i].pos.y = -pos.y;
			}

			// アルファ値の設定
			{
				float alpha = pPartState->alpha;
				if (pPartState->useLocalAlpha)
				{
					alpha = pPartState->alpha;
				}
				pBufferRaw->vertices[i].color.w = alpha;
			}
		}
	}

	//================================================================================
	Float2 AnimationController::calcOffsetLocalVertexPos(const AnimationPartState* pPartState, int32 index) const
	{
		if (pPartState == nullptr)
		{
			return Float2::Zero();
		}
		const auto* pBuffer2D = pPartState->pBuffer2D.get();
		auto* pMeshStateValue = std::get_if<AnimationPartStateMesh>(&(pPartState->partValue));
		if (pBuffer2D == nullptr
			or pMeshStateValue == nullptr)
		{
			return Float2::Zero();
		}
		if (index < 0 or pBuffer2D->vertices.size() < index)
		{
			return Float2::Zero();
		}
		const Mat4x4& matrix = pPartState->modelMatrix;
		Float2 offset = matrix.transformPoint(Float3{ pMeshStateValue->vertexPositions[index], 0.0f }).xy();
		Float2 tmp = offset + pMeshStateValue->worldVertexPositions[index];
		Mat4x4 invMat = matrix.inverse();
		tmp = invMat.transformPoint(Float3{ tmp, 0.0f }).xy();
		return Float2(tmp - pMeshStateValue->vertexPositions[index]);
	}

}
