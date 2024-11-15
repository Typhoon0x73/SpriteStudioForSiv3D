#include "SSEffectController.hpp"
#include "SSAnimationController.hpp"
#include "PartState/SSAnimationPartState.hpp"

namespace s3d::SpriteStudio
{
	//================================================================================
	EffectController::EffectController()
		: m_pProject{ nullptr }
		, m_pEffectModel{ nullptr }
		, m_pParentState{ nullptr }
		, m_isLoop{ false }
		, m_isStop{ true }
		, m_isInfinite{ false }
		, m_isNeedApply{ false }
		, m_frame{ -1 }
		, m_targetFrame{ -1 }
		, m_seed{ 0 }
		, m_layoutPosition{ Float3::Zero() }
		, m_layoutScale{ Float2::Zero() }
		, m_seedOffset{ 0 }
		, m_emitters{}
	{
	}

	//================================================================================
	EffectController::~EffectController()
	{
	}

	//================================================================================
	void EffectController::play()
	{
		m_isStop = false;
	}

	//================================================================================
	void EffectController::stop()
	{
		m_isStop = true;
	}

	//================================================================================
	void EffectController::apply()
	{
		// 停止中か更新の必要が無ければ即リターン
		if (m_isStop or not(m_isNeedApply))
		{
			return;
		}

		m_targetFrame = m_frame;
		m_isNeedApply = false;

		if (m_isInfinite
			or not(m_isLoop)
			or m_effectTimeLength <= m_frame
			)
		{
			return;
		}

		m_targetFrame = m_frame % m_effectTimeLength;
		int32 tmp = static_cast<int32>(m_frame / static_cast<float>(m_effectTimeLength));
		setSeedOffset(tmp);


		for (auto& pEmitter : m_updateEmitters)
		{
			if (pEmitter == nullptr)
			{
				continue;
			}
			pEmitter->seedOffset = m_seedOffset;

			if (pEmitter->pParent != nullptr)
			{
				// グローバルの時間で現在親がどれだけ生成されているのかをチェックする
				pEmitter->pParent->update(static_cast<double>(m_targetFrame), 0);
			}
		}
	}

	//================================================================================
	void EffectController::reload()
	{
		if (m_pEffectModel == nullptr)
		{
			return;
		}

		m_frame = 0;
		m_isNeedApply = true;

		stop();
		m_emitters.clear();

		const auto& nodes = m_pEffectModel->nodes;
		m_emitters.reserve(nodes.size());

		m_layoutScale.x = m_pEffectModel->layoutScaleX / 100.0f;
		m_layoutScale.y = m_pEffectModel->layoutScaleY / 100.0f;

		Array<int32> countNums{ nodes.size(), 0, std::allocator<int32>{} };

		// どれか一つでも無限生成可能なら設定する。
		bool isInfinite = false;
		for (const auto& node : nodes)
		{
			if (node.type == EffectNodeType::Emitter)
			{
				EffectEmitter emitter;
				emitter.parentIndex = node.parentIndex;
				// 繋ぎ先は恐らくパーティクルなのでエミッタに変換
				if (emitter.parentIndex != 0)
				{
					emitter.parentIndex = nodes[emitter.parentIndex].parentIndex;
				}
				countNums[emitter.parentIndex]++;
				if (10 < countNums[emitter.parentIndex])
				{
					continue; //子１０ノード表示制限
				}

				// 孫抑制対策
				if (emitter.parentIndex != 0)
				{
					int32 parentIndex = nodes[emitter.parentIndex].parentIndex;
					if (parentIndex != 0)
					{
						if (0 < nodes[parentIndex].parentIndex)
						{
							continue;
						}
					}
				}
				initEmitter(emitter, node);
				if (emitter.emitterParam.isInfinite)
				{
					isInfinite = true;
				}
				m_emitters.emplace_back(emitter);
			}
			else
			{
				m_emitters.push_back(EffectEmitter{});
			}
		}
		m_isInfinite = isInfinite;

		m_effectTimeLength = 0;
		//事前計算 updateListにルートの子を配置し親子関係を結ぶ
		for (size_t i = 0; i < m_emitters.size(); i++)
		{
			auto& emitter = m_emitters[i];
			emitter.uniqueId = static_cast<uint32>(i);
			emitter.preCalculate();

			const int32 parentIndex = emitter.parentIndex;

			if (emitter.parentIndex == 0) // ルート直下
			{
				emitter.pParent = nullptr;
				emitter.globalTime = emitter.getTimeLength();
				m_updateEmitters.emplace_back(&emitter);
			}
			else
			{
				EffectEmitter* pParent = &(m_emitters[parentIndex]);
				if (pParent == nullptr)
				{
					// nullptrはおかしい。
					return;
				}

				emitter.pParent = pParent;

				emitter.globalTime = static_cast<size_t>(emitter.getTimeLength()) + static_cast<size_t>(pParent->getTimeLength());

				m_updateEmitters.emplace_back(&emitter);
			}

			if (m_effectTimeLength < emitter.globalTime)
			{
				m_effectTimeLength = emitter.globalTime;
			}
		}
		// プライオリティソート
		m_updateEmitters.sort_by(
			[](const auto& left, const auto& right)
			{
				if (left->priority == right->priority)
				{
					if (left->uniqueId < right->uniqueId)
					{
						return true;
					}
				}
				return left->priority < right->priority;
			}
		);
	}

	//================================================================================
	void EffectController::setProject(const Project* pProject)
	{
		m_pProject = pProject;
	}

	//================================================================================
	void EffectController::setEffectModel(const EffectModel* pEffectModel)
	{
		m_pEffectModel = pEffectModel;
		reload();
	}

	//================================================================================
	void EffectController::setSeed(uint32 seed)
	{
		m_seed = seed * SeedMagic;
	}

	//================================================================================
	void EffectController::setSeedOffset(int32 offset)
	{
		if (m_pEffectModel == nullptr)
		{
			return;
		}
		if (m_pEffectModel->lockRandomSeedOpt.has_value())
		{
			m_seedOffset = 0;
		}
		else
		{
			m_seedOffset = offset;
		}
	}

	//================================================================================
	void EffectController::setFrame(int32 frame)
	{
		if (m_frame == frame)
		{
			return;
		}
		m_frame = frame;
		m_isNeedApply = true;
	}

	//================================================================================
	void EffectController::setParentPartState(const AnimationPartState* pState)
	{
		m_pParentState = pState;
	}

	//================================================================================
	void EffectController::setLoop(LoopEnable enable)
	{
		m_isLoop = enable.getBool();
	}

	//================================================================================
	void EffectController::initEmitter(EffectEmitter& out, const EffectNode& src)
	{
		if (const auto& behavior = src.behaviorOpt)
		{
			out.pBehavior = &(behavior.value());
		}
		if (m_pProject != nullptr
			and out.pBehavior != nullptr)
		{
			const String& cellmapName = out.pBehavior->cellmapName;
			const String& cellName = out.pBehavior->cellName;
			out.pCell = m_pProject->findCell(cellmapName, cellName);
		}

		// 各パラメータからエミッタを初期化する。
		for (const auto& elementVariant : out.pBehavior->elements)
		{
			std::visit(
				[&](auto& element)
				{
					element.initialize(out);
				},
				elementVariant
			);
		}

		out.emitterSeed = m_seed;

		if (const auto& randomSeed = out.particleParam.overrideRandomSeedOpt)
		{
			out.emitterSeed = randomSeed.value();
		}
		else if (const auto& lockRandomSeed = m_pEffectModel->lockRandomSeedOpt)
		{
			out.emitterSeed = (lockRandomSeed.value() + 1) * SeedMagic;
		}

		out.emitterParam.life += static_cast<int32>(out.particleParam.delay); // ディレイ分加算
	}

}
