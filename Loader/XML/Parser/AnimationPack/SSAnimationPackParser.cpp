#include "SSAnimationPackParser.hpp"
#include "../../../../Common/SSOutputDebugLog.hpp"
#include "../../SSXMLEnumTable.hpp"
#include "../SSXMLParserUtilities.hpp"

namespace s3d::SpriteStudio::XMLParser
{
	namespace
	{
		//================================================================================
		bool ParseAnimationSetting(const XMLElement& element, AnimationSetting& out)
		{
			if (element.name() != U"settings")
			{
				return false;
			}
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToInt32(child, U"fps", out.fps))
				{
					continue;
				}
				if (Utilities::TextToInt32(child, U"frameCount", out.frameCount))
				{
					continue;
				}
				if (Utilities::TextToSize(child, U"canvasSize", out.canvasSize))
				{
					continue;
				}
				if (Utilities::TextToFloat2(child, U"pivot", out.pivot))
				{
					continue;
				}
				if (Utilities::TextToEnum(child, U"sortMode", PartsSortModeTable, out.sortMode))
				{
					continue;
				}
				if (Utilities::TextToInt32(child, U"startFrame", out.startFrame))
				{
					continue;
				}
				if (Utilities::TextToInt32(child, U"endFrame", out.endFrame))
				{
					continue;
				}
			}

			// Ver5.8以前はstartFrame,endFrameのデータが無いらしい。
			// 重要かつ実装コストもほぼゼロなので対応しておく。
			if (out.startFrame == 0
				and out.endFrame == 0)
			{
				out.endFrame = out.frameCount - 1;
			}

			return true;
		}

		//================================================================================
		bool ParseModelPartVariantValue(const XMLElement& element, PartType type, ModelPartInfoVariant& out)
		{
			switch (type)
			{
			case PartType::Null:
			{
				ModelPartInfoNull nullInfo;
				out = nullInfo;
				return true;
			}
			case PartType::Normal:
			{
				ModelPartInfoNormal normalInfo;
				out = normalInfo;
				return true;
			}
			case PartType::Text:
			{
				ModelPartInfoText textInfo;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{

				}
				out = textInfo;
				return true;
			}
			case PartType::Instance:
			{
				ModelPartInfoInstance instanceInfo;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{
					if (Utilities::TextToString(child, U"refAnimePack", instanceInfo.refAnimationPackName))
					{
						continue;
					}
					if (Utilities::TextToString(child, U"refAnime", instanceInfo.refAnimationName))
					{
						continue;
					}
				}
				out = instanceInfo;
				return true;
			}
			case PartType::Armature:
			{
				ModelPartInfoArmature armatureInfo;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{
					if (Utilities::TextToFloat2(child, U"bonePosition", armatureInfo.bonePosition))
					{
						continue;
					}
					if (Utilities::TextToFloat(child, U"boneRotation", armatureInfo.boneRotation))
					{
						continue;
					}
				}
				out = armatureInfo;
				return true;
			}
			case PartType::Effect:
			{
				ModelPartInfoEffect effectInfo;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{
					if (Utilities::TextToString(child, U"refEffectName", effectInfo.refEffectName))
					{
						continue;
					}
				}
				out = effectInfo;
				return true;
			}
			case PartType::Mesh:
			{
				ModelPartInfoMesh meshInfo;
				out = meshInfo;
				return true;
			}
			case PartType::Movenode:
			{
				ModelPartInfoMovenode movenodeInfo;
				out = movenodeInfo;
				return true;
			}
			case PartType::Constraint:
			{
				ModelPartInfoConstraint constraintInfo;
				out = constraintInfo;
				return true;
			}
			case PartType::Mask:
			{
				ModelPartInfoMask maskInfo;
				out = maskInfo;
				return true;
			}
			case PartType::Joint:
			{
				ModelPartInfoJoint jointInfo;
				out = jointInfo;
				return true;
			}
			case PartType::Bonepoint:
			{
				ModelPartInfoBonepoint bonepointInfo;
				out = bonepointInfo;
				return true;
			}
			case PartType::Shape:
			{
				ModelPartInfoShape shapeInfo;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{

				}
				out = shapeInfo;
				return true;
			}
			case PartType::Nineslice:
			{
				ModelPartInfoNineslice ninesliceInfo;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{

				}
				out = ninesliceInfo;
				return true;
			}

			// 知らない,無効なパーツ
			case PartType::Invalid:
			default:
			{
				break;
			}
			}
			return false;
		}

		//================================================================================
		bool ParseModelPart(const XMLElement& element, AnimationModelPart& out)
		{
			if (element.name() != U"value")
			{
				return false;
			}

			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToString(child, U"name", out.name))
				{
					continue;
				}
				if (Utilities::TextToInt32(child, U"arrayIndex", out.arrayIndex))
				{
					continue;
				}
				if (Utilities::TextToInt32(child, U"parentIndex", out.parentIndex))
				{
					continue;
				}
				if (Utilities::TextToEnum(child, U"type", PartTypeTable, out.partType))
				{
					continue;
				}
				if (Utilities::TextToEnum(child, U"boundsType", BoundsTypeTable, out.boundsType))
				{
					continue;
				}
				if (Utilities::TextToEnum(child, U"inheritType", InheritTypeTable, out.inheritType))
				{
					continue;
				}
				if (Utilities::TextToEnum(child, U"alphaBlendType", BlendTypeTable, out.blendType))
				{
					continue;
				}
				if (Utilities::TextToBool(child, U"maskInfluence", out.isMaskInfluence))
				{
					continue;
				}
			}

			if (not(ParseModelPartVariantValue(element, out.partType, out.partVariantValue)))
			{
				// 知らないパーツ
				return false;
			}

			return true;
		}

		//================================================================================
		bool ParseModelPartList(const XMLElement& element, Array<AnimationModelPart>& out)
		{
			if (element.name() != U"partList")
			{
				return false;
			}

			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				AnimationModelPart modelPart;
				if (ParseModelPart(child, modelPart))
				{
					out.emplace_back(modelPart);
				}
			}
			return true;
		}

		//================================================================================
		bool ParseModelMeshBind(const XMLElement& element, AnimationModelMeshBind& out)
		{
			if (element.name() != U"value")
			{
				return false;
			}

			const auto& lines = element.text().split(U',');
			for (const auto& line : lines)
			{
				const auto& infoLines = line.split(U' ');
				const size_t dataCount = infoLines.size();
				if (dataCount == 0)
				{
					// カンマ区切りデータはあるのに、情報なしはおかしい。
					return false;
				}
				int32 infoIndex = 0;
				out.bindBoneNum = Parse<int32>(infoLines[infoIndex++]);
				for (int32 i = 0; i < out.bindBoneNum; i++)
				{
					if ((dataCount <= infoIndex)
						or (MeshBindBoneMax <= i))
					{
						break;
					}
					AnimationModelMeshBind::Info bindInfo;
					bindInfo.boneIndex = ParseOr<int32, int32>(infoLines[infoIndex++], 0);
					bindInfo.weight = ParseOr<int32, int32>(infoLines[infoIndex++], 0);
					bindInfo.offset.x = ParseOr<float, float>(infoLines[infoIndex++], 0.0f);
					bindInfo.offset.y = ParseOr<float, float>(infoLines[infoIndex++], 0.0f);
					out.vertexBinds.emplace_back(bindInfo);
				}
			}
			return true;
		}

		//================================================================================
		bool ParseModelMeshBindList(const XMLElement& element, Array<AnimationModelMeshBind>& out)
		{
			if (element.name() != U"meshList")
			{
				return false;
			}

			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				AnimationModelMeshBind meshBind;
				if (ParseModelMeshBind(child, meshBind))
				{
					out.emplace_back(meshBind);
				}
			}
			return true;
		}

		//================================================================================
		bool ParseModel(const XMLElement& element, AnimationModel& out)
		{
			if (element.name() != U"Model")
			{
				return false;
			}
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseModelPartList(child, out.parts))
				{
					continue;
				}
				if (ParseModelMeshBindList(child, out.meshBinds))
				{
					continue;
				}
			}
			return true;
		}

		//================================================================================
		bool ParseAnimationLabel(const XMLElement& element, Label& out)
		{
			if (element.name() != U"value")
			{
				return false;
			}

			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToInt32(child, U"time", out.frame))
				{
					continue;
				}
				if (Utilities::TextToString(child, U"name", out.name))
				{
					continue;
				}
			}
			return true;
		}

		//================================================================================
		bool ParseAnimationLabels(const XMLElement& element, Array<Label>& out)
		{
			if (element.name() != U"labels")
			{
				return false;
			}

			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				Label label;
				if (ParseAnimationLabel(child, label))
				{
					out.emplace_back(label);
				}
			}
			return true;
		}

		//================================================================================
		bool ParseAnimationKeyFrames(const XMLElement& element, Array<AnimationKeyFrame>& out)
		{
			if (element.name() != U"key")
			{
				return false;
			}
		}

		//================================================================================
		bool ParseAnimationAttribute(const XMLElement& element, AnimationAttribute& out)
		{
			if (element.name() != U"attribute")
			{
				return false;
			}

			if (Utilities::AttributeToEnum(element, U"tag", AttributeKindTable, out.kind))
			{
				// アトリビュートのタグが見つからない場合は未対応項目がある。
				return false;
			}

			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseAnimationKeyFrames(child, out.keyFrames))
				{
					continue;
				}
			}

			return true;
		}

		//================================================================================
		bool ParseAnimationAttributes(const XMLElement& element, Array<AnimationAttribute>& out)
		{
			if (element.name() != U"attributes")
			{
				return false;
			}

			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				AnimationAttribute attribute;
				if (ParseAnimationAttribute(child, attribute))
				{
					out.emplace_back(attribute);
				}
			}

			return true;
		}

		//================================================================================
		bool ParseAnimationPart(const XMLElement& element, AnimationPart& out)
		{
			if (element.name() != U"partAnime")
			{
				return false;
			}

			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToString(child, U"partName", out.name))
				{
					continue;
				}
				if (ParseAnimationAttributes(child, out.attributes))
				{
					continue;
				}
			}
			return true;
		}

		//================================================================================
		bool ParseAnimationParts(const XMLElement& element, Array<AnimationPart>& out)
		{
			if (element.name() != U"partAnimes")
			{
				return false;
			}

			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				AnimationPart part;
				if (ParseAnimationPart(child, part))
				{
					out.emplace_back(part);
				}
			}
			return true;
		}

		//================================================================================
		bool ParseAnimation(const XMLElement& element, Animation& out)
		{
			if (element.name() != U"anime")
			{
				return false;
			}

			AnimationSetting overrideSetting;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToString(child, U"name", out.name))
				{
					continue;
				}
				if (ParseAnimationSetting(child, overrideSetting))
				{
					out.overrideSetting = overrideSetting;
					continue;
				}
				if (ParseAnimationLabels(child, out.labels))
				{
					continue;
				}
				if (ParseAnimationParts(child, out.animationParts))
				{
					continue;
				}
				if (Utilities::TextToBool(child, U"isSetup", out.isSetup))
				{
					continue;
				}
			}

			return true;
		}

		//================================================================================
		bool ParseAnimationList(const XMLElement& element, Array<Animation>& out)
		{
			if (element.name() != U"animeList")
			{
				return false;
			}

			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				Animation animation;
				if (ParseAnimation(child, animation))
				{
					out.emplace_back(animation);
				}
			}
			return true;
		}

		//================================================================================
		bool ParseAnimationPack(const XMLElement& element, AnimationPack& out)
		{
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseAnimationSetting(child, out.setting))
				{
					continue;
				}
				if (Utilities::TextToString(child, U"name", out.name))
				{
					continue;
				}
				if (ParseModel(child, out.model))
				{
					continue;
				}
				if (Utilities::ChildToFilePathArray(child, U"cellmapNames", U"value", out.cellmapNames))
				{
					continue;
				}
				if (ParseAnimationList(child, out.animations))
				{
					continue;
				}
			}
			return true;
		}
	}

	//================================================================================
	bool AnimationPackParser(const XMLElement& element, AnimationPack& out)
	{
		try
		{
			return ParseAnimationPack(element, out);
		}
		catch (const Error& e)
		{
			DebugLog::Print(DebugLog::LogType::Error, U"解析中にエラーが発生しました。");
			DebugLog::Print(DebugLog::LogType::Error, U"Type:"_fmt(e.type()));
			DebugLog::Print(DebugLog::LogType::Error, U"What:"_fmt(e.what()));
			return false;
		}
	}

}
