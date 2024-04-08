#include "SSAnimationPartStateBuilder.hpp"
#include "../SSAnimationController.hpp"
#include "../../Common/SSOutputDebugLog.hpp"

namespace s3d::SpriteStudio
{
	namespace
	{
		//================================================================================
		bool MakeMesh(const Cell* pCell, const Texture* pTexture, Buffer2D* pBuffer2D, AnimationPartStateMesh& out)
		{
			if (
				(pBuffer2D == nullptr)                   // バッファも必要。
				or (pCell == nullptr)                    // セル情報からメッシュを生成するので無ければ無理。
				or (not(pCell->meshInfoOpt.has_value())) // メッシュ情報無かったら作成できない。
				or (pTexture == nullptr)                 // 画像データも欲しい。
				)
			{
				DebugLog::Print(DebugLog::LogType::Error, U"データ不十分の為作成できませんでした。");
				return false;
			}

			// メッシュ情報から作成する。
			const auto& meshInfo = pCell->meshInfoOpt.value();
			const auto& vertices = meshInfo.points;
			const auto& indices = meshInfo.triangles;
			const size_t vertexCount = vertices.size();
			out.vertexPositions.clear();
			out.worldVertexPositions.clear();
			pBuffer2D->vertices.clear();
			pBuffer2D->indices.clear();
			out.vertexPositions.reserve(vertexCount);
			out.worldVertexPositions.reserve(vertexCount);
			pBuffer2D->vertices.reserve(vertexCount);
			pBuffer2D->indices.reserve(vertexCount);

			out.bindBoneInfomations.clear();
			out.bindBoneInfomations.resize(vertices.size());

			// UV,座標計算
			const auto& cellPos = pCell->rect.pos;
			const auto& cellSize = pCell->rect.size;
			const auto& cellPivot = pCell->pivot;
			Float2 offs = Float2::Zero(); // 中央
			offs.x = static_cast<float>(-cellSize.x * 0.5f);
			offs.y = static_cast<float>(cellSize.y * 0.5f);

			offs.x -= cellPivot.x * cellSize.x;
			offs.y -= cellPivot.y * cellSize.y;

			const float uvPixelX = 1.0f / static_cast<float>(pTexture->width());
			const float uvPixelY = 1.0f / static_cast<float>(pTexture->height());

			for (const auto& it : vertices)
			{
				Vertex2D vertex = Vertex2D();
				vertex.color = Float4::One();
				vertex.pos.x = it.x + offs.x;
				vertex.pos.y = -it.y + offs.y;
				vertex.tex.x = (cellPos.x + it.x) * uvPixelX;
				vertex.tex.y = (cellPos.y + it.y) * uvPixelY;
				pBuffer2D->vertices.emplace_back(vertex);
				out.vertexPositions.emplace_back(vertex.pos);
				out.worldVertexPositions.emplace_back(Float2::Zero());
			}
			for (const auto& it : indices)
			{
				pBuffer2D->indices.emplace_back(it);
			}

			return true;
		}

		//================================================================================
		bool MakePartStateValue(const Project* pProject, PartType partType, AnimationPartState* pOutPartState)
		{
			auto& partValueRaw = pOutPartState->partValue;

			// ノーマルパーツ
			if (partType == PartType::Normal)
			{
				pOutPartState->pBuffer2D = std::make_unique<Buffer2D>(4U, 2U);
				auto* pBuffer2D = pOutPartState->pBuffer2D.get();
				pBuffer2D->indices[0] = { static_cast<uint16>(0), static_cast<uint16>(1), static_cast<uint16>(2) };
				pBuffer2D->indices[1] = { static_cast<uint16>(2), static_cast<uint16>(1), static_cast<uint16>(3) };
				// カラー値の初期化
				for (auto& vertex : pBuffer2D->vertices)
				{
					vertex.color = Float4::One();
				}
			}

			// インスタンスパーツの場合、アニメーションを作成する
			if (partType == PartType::Instance)
			{
				partValueRaw = AnimationPartStateInstance();
				auto& valueRaw = std::get<AnimationPartStateInstance>(partValueRaw);

				// インスタンスパーツ情報を取得する。
				const auto* pModelPart = pOutPartState->pAnimationModelPart;
				if (not(std::holds_alternative<ModelPartInfoInstance>(pModelPart->partVariantValue)))
				{
					// インスタンスパーツ情報でない？？？
					DebugLog::Print(DebugLog::LogType::Notice, U"インスタンスパーツタイプですが、データが正しく入っていません。");
					return false;
				}

				// インスタンスパーツ情報から、アニメーションを作成する。
				const auto& modelPartInfo = std::get<ModelPartInfoInstance>(pModelPart->partVariantValue);
				const auto& instancePackName = modelPartInfo.refAnimationPackName;
				const auto& instanceAnimName = modelPartInfo.refAnimationName;
				valueRaw.pRefAnimationController = std::make_unique<AnimationController>();
				if (not(valueRaw.pRefAnimationController->build(pProject, instancePackName, instanceAnimName)))
				{
					// アニメーションの作成に失敗した。
					DebugLog::Print(DebugLog::LogType::Error, U"インスタンスパーツのアニメーション作成に失敗しました。");
					return false;
				}
				// 親子付け
				auto& instancePartStatesRaw = valueRaw.pRefAnimationController->getPartStatesRaw();
				if (not(instancePartStatesRaw.isEmpty()))
				{
					// 先頭はrootのはずなので、rootの親に自分をつける。
					instancePartStatesRaw[0]->pParent = pOutPartState;
				}
				return true;
			}

			// エフェクトパーツ
			if (partType == PartType::Effect)
			{
				if (pOutPartState->pAnimationModelPart == nullptr)
				{
					// モデルパーツがない。
					return false;
				}
				// エフェクトパーツの情報を取得する。
				const auto* pEffectInfo = std::get_if<ModelPartInfoEffect>(&(pOutPartState->pAnimationModelPart->partVariantValue));
				if (pEffectInfo == nullptr)
				{
					return false;
				}
				// エフェクトデータを取得する。
				const Effect* pEffect = pProject->findEffect(pEffectInfo->refEffectName);
				if (pEffect == nullptr)
				{
					return false;
				}
				auto* pEffectpartState = std::get_if<AnimationPartStateEffect>(&(pOutPartState->partValue));
				if (pEffectpartState == nullptr)
				{
					return false;
				}
				auto& effectControllerRaw = pEffectpartState->effectController;
				effectControllerRaw.setParentPartState(pOutPartState);
				effectControllerRaw.setEffectModel(&pEffect->model);
				effectControllerRaw.setSeed(RandomUint32());
				effectControllerRaw.reload();
				effectControllerRaw.stop();
				effectControllerRaw.setLoop(LoopEnable::No);
				return true;
			}

			// メッシュパーツ
			if (partType == PartType::Mesh)
			{
				pOutPartState->pBuffer2D.reset();
				pOutPartState->pBuffer2D = std::make_unique<Buffer2D>();
				partValueRaw = AnimationPartStateMesh();
				auto& valueRaw = std::get<AnimationPartStateMesh>(partValueRaw);
				if (not(MakeMesh(pOutPartState->cellmapTextureInfo.pCell, pOutPartState->cellmapTextureInfo.pTexture, pOutPartState->pBuffer2D.get(), valueRaw)))
				{
					// メッシュを作成できなかった。
					DebugLog::Print(DebugLog::LogType::Error, U"メッシュパーツの作成に失敗しました。");
					return false;
				}
				return true;
			}

			// その他特に何もしないパーツ達
			return true;
		}

		//================================================================================
		bool MakeBoundsValue(BoundsType type, AnimationPartState* pOutPartState)
		{
			if (type == BoundsType::Invalid or type == BoundsType::None)
			{
				// 生成せず完了。
				pOutPartState->pBoundsValue.reset();
				return true;
			}
			pOutPartState->pBoundsValue = std::make_unique<BoundsValueVariant>();
			BoundsValueVariant& boundsValueRaw = (*pOutPartState->pBoundsValue.get());
			switch (type)
			{
			case BoundsType::Quad:
			{
				boundsValueRaw = Quad();
				break;
			}
			case BoundsType::AABB:
			{
				boundsValueRaw = RectF();
				break;
			}
			case BoundsType::Circle:
			case BoundsType::CircleScaleMin:
			case BoundsType::CircleScaleMax:
			{
				boundsValueRaw = Circle();
				break;
			}
			default:
				// 知らない種類がきた。
				DebugLog::Print(DebugLog::LogType::Notice, U"知らない当たり判定の種類です。:{}"_fmt(FromEnum(type)));
				return false;
			}
			return true;
		}
	}


	//================================================================================
	AnimationPartState* AnimationPartStateBuilder::Build(const Project* pProject, const AnimationModelPart* pModelPart, const AnimationPart* pSetupPart, const AnimationPart* pAnimationPart, const CellmapTextureInfo& cellmapTextureInfo)
	{
		// モデルパーツがnullptrということはありえない。
		// プロジェクトデータがない場合は作成できないものがある。
		if (pModelPart == nullptr or pProject == nullptr)
		{
			DebugLog::Print(DebugLog::LogType::Error, U"データ不十分により作成できません。");
			return nullptr;
		}
		AnimationPartState* pState = new AnimationPartState();
		pState->pAnimationModelPart = pModelPart;
		pState->pSetupAnimationPart = pSetupPart; // nullptrの場合もある
		pState->pAnimationPart = pAnimationPart;  // nullptrの場合もある
		pState->cellmapTextureInfo = cellmapTextureInfo;

		// アルファ値の継承率を設定
		pState->alphaInheritRate = pModelPart->alphaIheritRate;

		// パーツごとに生成する
		if (not(MakePartStateValue(pProject, pModelPart->partType, pState)))
		{
			// パーツ作成に失敗した。
			if (pState != nullptr)
			{
				delete pState;
				pState = nullptr;
			}
			DebugLog::Print(DebugLog::LogType::Error, U"パーツの作成に失敗しました。");
			return nullptr;
		}

		// 当たり判定形状を作成する
		if (not(MakeBoundsValue(pModelPart->boundsType, pState)))
		{
			// 当たり判定形状の作成に失敗。
			if (pState != nullptr)
			{
				delete pState;
				pState = nullptr;
			}
			DebugLog::Print(DebugLog::LogType::Error, U"当たり判定形状の作成に失敗しました。");
			return nullptr;
		}

		return pState;
	}

}
