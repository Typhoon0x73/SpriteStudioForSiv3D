#include "SSAnimationPartStateBuilder.hpp"
#include "../SSAnimationController.hpp"

namespace s3d::SpriteStudio
{
	namespace
	{
		//================================================================================
		bool MakeMesh(const Cell* pCell, const Texture* pTexture, AnimationPartStateMesh& out)
		{
			if (
				(pCell == nullptr)                       // セル情報からメッシュを生成するので無ければ無理。
				or (not(pCell->meshInfoOpt.has_value())) // メッシュ情報無かったら作成できない。
				or (pTexture == nullptr)                 // 画像データも欲しい。
				)
			{
				return false;
			}

			// メッシュ情報から作成する。
			const auto& meshInfo = pCell->meshInfoOpt.value();
			const auto& vertices = meshInfo.points;
			const auto& indices = meshInfo.triangles;
			const size_t vertexCount = vertices.size();
			out.vertexPositions.clear();
			out.worldVertexPositions.clear();
			out.buffer2D.vertices.clear();
			out.buffer2D.indices.clear();
			out.vertexPositions.reserve(vertexCount);
			out.worldVertexPositions.reserve(vertexCount);
			out.buffer2D.vertices.reserve(vertexCount);
			out.buffer2D.indices.reserve(vertexCount);

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
				out.buffer2D.vertices.emplace_back(vertex);
				out.vertexPositions.emplace_back(vertex.pos);
				out.worldVertexPositions.emplace_back(Float2::Zero());
			}
			for (const auto& it : indices)
			{
				out.buffer2D.indices.emplace_back(it);
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
				partValueRaw = AnimationPartStateNormal();
				auto& valueRaw = std::get<AnimationPartStateNormal>(partValueRaw);
				valueRaw.buffer2D.indices[0] = { static_cast<uint16>(0), static_cast<uint16>(1), static_cast<uint16>(2) };
				valueRaw.buffer2D.indices[1] = { static_cast<uint16>(2), static_cast<uint16>(1), static_cast<uint16>(3) };
				// カラー値の初期化
				for (auto& vertex : valueRaw.buffer2D.vertices)
				{
					vertex.color = Float4::One();
				}
				return true;
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

			// メッシュパーツ
			if (partType == PartType::Mesh)
			{
				partValueRaw = AnimationPartStateMesh();
				auto& valueRaw = std::get<AnimationPartStateMesh>(partValueRaw);
				if (not(MakeMesh(pOutPartState->pCell, pOutPartState->pTexture, valueRaw)))
				{
					// メッシュを作成できなかった。
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
				return false;
			}
			return true;
		}
	}


	//================================================================================
	AnimationPartState* AnimationPartStateBuilder::Build(const Project* pProject, const AnimationModelPart* pModelPart, const AnimationPart* pSetupPart, const AnimationPart* pAnimationPart, const Cell* pCell, const Texture* pTexture)
	{
		// モデルパーツがnullptrということはありえない。
		// プロジェクトデータがない場合は作成できないものがある。
		if (pModelPart == nullptr or pProject == nullptr)
		{
			return nullptr;
		}
		AnimationPartState* pState = new AnimationPartState();
		pState->pAnimationModelPart = pModelPart;
		pState->pSetupAnimationPart = pSetupPart; // nullptrの場合もある
		pState->pAnimationPart = pAnimationPart;  // nullptrの場合もある
		pState->pCell = pCell;
		pState->pTexture = pTexture;

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
			return nullptr;
		}

		return pState;
	}

}
