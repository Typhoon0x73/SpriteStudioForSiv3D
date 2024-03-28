#include "SSAnimationController.hpp"
#include "PartState/SSAnimationPartStateBuilder.hpp"

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

			AnimationPartState* pStateRaw = AnimationPartStateBuilder::Build(pProject, pModelPart, pSetupPart, pAnimationPart);
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

		// パーツの存在確認
		const auto* pParent = pPartState->pParent;
		const auto* pModelPart = pPartState->pAnimationModelPart;
		const auto* pSetupPart = pPartState->pSetupAnimationPart;
		const auto* pAnimationPart = pPartState->pAnimationPart;
		const bool existParent = (pParent != nullptr);
		const bool existSetupPart = (pSetupPart != nullptr);
		const bool existAnimationPart = (pAnimationPart == nullptr);
		const bool existModelPart = not(pModelPart == nullptr);

		// セットアップもアニメーションも無ければ表示しないフラグを立てる。
		if (not(existSetupPart) and not(existAnimationPart))
		{
			pPartState->isHide = true;
		}

		// 初期化しておく
		initPartState(pPartState);
	}

}
