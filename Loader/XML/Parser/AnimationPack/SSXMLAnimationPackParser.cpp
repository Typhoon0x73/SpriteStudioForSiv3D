#include "SSXMLAnimationPackParser.hpp"
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

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation settings ---");
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
			// データがあっても両方0,0のこともあるので、対応する。
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

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation model part value ---");
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
				DebugLog::Print(DebugLog::LogType::Notice, U"未対応のパーツが検知されました。");
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

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation model part list ---");
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

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation model mesh bind ---");
			const auto& lines = element.text().split(U',');
			for (const auto& line : lines)
			{
				const auto& infoLines = line.split(U' ');
				const size_t dataCount = infoLines.size();
				if (dataCount == 0)
				{
					// データが無い場合はスキップ
					continue;
				}
				int32 infoIndex = 0;
				out.bindBoneNum = Parse<int32>(infoLines[infoIndex++]);
				for (int32 i = 0; i < out.bindBoneNum; i++)
				{
					if ((dataCount <= infoIndex)
						or (MeshBindBoneMax <= i))
					{
						// データが溢れている。
						DebugLog::Print(DebugLog::LogType::Error, U"メッシュバインドの情報が想定より多いです。");
						return false;
					}
					AnimationModelMeshBind::BindBoneInfomations bindInfomations;
					bindInfomations[i].boneIndex = ParseOr<int32, int32>(infoLines[infoIndex++], 0);
					bindInfomations[i].weight = ParseOr<int32, int32>(infoLines[infoIndex++], 0);
					bindInfomations[i].offset.x = ParseOr<float, float>(infoLines[infoIndex++], 0.0f);
					bindInfomations[i].offset.y = ParseOr<float, float>(infoLines[infoIndex++], 0.0f);
					out.vertexBinds.emplace_back(bindInfomations);
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

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation model mesh bind list ---");
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

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation model ---");
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

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation label ---");
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

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation labels ---");
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
		bool ParseAnimationCurve(const XMLElement& element, AnimationCurve& out)
		{
			if (element.name() != U"curve")
			{
				return false;
			}
			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation curve ---");
			DebugLog::Print(DebugLog::LogType::Verbose, U"text:{}"_fmt(element.text()));

			auto lines = element.text().split(U' ');
			if (lines.size() < 4)
			{
				// データが４つ無いとおかしい。
				DebugLog::Print(DebugLog::LogType::Error, U"カーブパラメータが足りません。");
				return false;
			}

			out.startFrame = Parse<float>(lines[0]);
			out.startValue = Parse<float>(lines[1]);
			out.endFrame = Parse<float>(lines[2]);
			out.endValue = Parse<float>(lines[3]);

			DebugLog::Print(DebugLog::LogType::Verbose, U"startFrame:{}"_fmt(out.startFrame));
			DebugLog::Print(DebugLog::LogType::Verbose, U"startValue:{}"_fmt(out.startValue));
			DebugLog::Print(DebugLog::LogType::Verbose, U"endFrame  :{}"_fmt(out.endFrame));
			DebugLog::Print(DebugLog::LogType::Verbose, U"endValue  :{}"_fmt(out.endValue));

			return true;
		}

		//================================================================================
		bool ParseAnimationPartsColorBlendColorValue(const XMLElement& element, AttributeValuePartsColor::BlendColorValue& out)
		{
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToColor(element, U"rgba", out.rgba))
				{
					continue;
				}
				if (Utilities::TextToFloat(element, U"rate", out.rate))
				{
					continue;
				}
			}
			return true;
		}

		//================================================================================
		bool ParseAnimationSignalCommandParam(const XMLElement& element, AttributeValueSignal::CommandParam& out)
		{
			if (element.name() != U"params")
			{
				return false;
			}

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation signal command param ---");
			String valueText = U"0";
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToString(child, U"paramId", out.id))
				{
					continue;
				}
				if (Utilities::TextToEnum(child, U"type", SignalTypeTable, out.type))
				{
					continue;
				}
				if (Utilities::TextToString(child, U"value", valueText))
				{
					continue;
				}
			}

			switch (out.type)
			{
			case SignalType::Index:
			case SignalType::Integer:
			{
				out.value = Parse<int32>(valueText);
				break;
			}
			case SignalType::Float:
			{
				out.value = Parse<float>(valueText);
				break;
			}
			case SignalType::Text:
			{
				out.value = valueText;
				break;
			}
			case SignalType::Invalid:
			case SignalType::None:
			default:
			{
				out.value = std::monostate{};
				break;
			}
			}
			return true;
		}

		//================================================================================
		bool ParseAnimationSignalCommand(const XMLElement& element, AttributeValueSignal::Command& out)
		{
			if (element.name() != U"commands")
			{
				return false;
			}

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation signal command ---");
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToBool(child, U"active", out.isActive))
				{
					continue;
				}
				if (Utilities::TextToString(child, U"commandId", out.id))
				{
					continue;
				}
				if (Utilities::TextToString(child, U"note", out.note))
				{
					continue;
				}

				AttributeValueSignal::CommandParam param;
				if (ParseAnimationSignalCommandParam(child, param))
				{
					out.params.emplace_back(param);
					continue;
				}
			}
			return true;
		}

		//================================================================================
		bool ParseAnimationKeyFrame(const XMLElement& element, AttributeKind kind, AnimationKeyFrame& out)
		{
			if (element.name() != U"value")
			{
				return false;
			}

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation keyframe ---");
			switch (kind)
			{
			case AttributeKind::Cell:
			{
				AttributeValueCell cellValue;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{
					if (Utilities::TextToInt32(child, U"mapId", cellValue.mapId))
					{
						continue;
					}
					if (Utilities::TextToString(child, U"name", cellValue.name))
					{
						continue;
					}
				}
				out.attributeVariantValue = cellValue;
				return true;
			}

			// float
			case AttributeKind::PosX:
			case AttributeKind::PosY:
			case AttributeKind::PosZ:
			case AttributeKind::RotateX:
			case AttributeKind::RotateY:
			case AttributeKind::RotateZ:
			case AttributeKind::ScaleX:
			case AttributeKind::ScaleY:
			case AttributeKind::LocalScaleX:
			case AttributeKind::LocalScaleY:
			case AttributeKind::Alpha:
			case AttributeKind::LocalAlpha:
			case AttributeKind::PivotX:
			case AttributeKind::PivotY:
			case AttributeKind::AnchorX:
			case AttributeKind::AnchorY:
			case AttributeKind::SizeX:
			case AttributeKind::SizeY:
			case AttributeKind::UVTranslateX:
			case AttributeKind::UVTranslateY:
			case AttributeKind::UVRotateZ:
			case AttributeKind::UVScaleX:
			case AttributeKind::UVScaleY:
			case AttributeKind::BoundRadius:
			{
				float value;
				if (not(Utilities::TextToFloat(element, U"value", value)))
				{
					return false;
				}
				out.attributeVariantValue = value;
				return true;
			}

			// int32
			case AttributeKind::Priority:
			case AttributeKind::Mask:
			{
				int32 value;
				if (not(Utilities::TextToInt32(element, U"value", value)))
				{
					return false;
				}
				out.attributeVariantValue = value;
				return true;
			}

			// bool
			case AttributeKind::FlipH:
			case AttributeKind::FlipV:
			case AttributeKind::Hide:
			case AttributeKind::ImageFlipH:
			case AttributeKind::ImageFlipV:
			{
				bool value;
				if (not(Utilities::TextToBool(element, U"value", value)))
				{
					return false;
				}
				out.attributeVariantValue = value;
				return true;
			}

			case AttributeKind::PartsColor:
			{
				AttributeValuePartsColor partsColorValue;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{
					if (Utilities::TextToEnum(child, U"target", ColorBlendTargetTable, partsColorValue.target))
					{
						continue;
					}
					if (Utilities::TextToEnum(child, U"blendType", BlendTypeTable, partsColorValue.blendType))
					{
						continue;
					}
					if ((child.name() == U"LT" or child.name() == U"color")
						and (ParseAnimationPartsColorBlendColorValue(child, partsColorValue.colors[0])))
					{
						continue;
					}
					if ((child.name() == U"RT")
						and (ParseAnimationPartsColorBlendColorValue(child, partsColorValue.colors[1])))
					{
						continue;
					}
					if ((child.name() == U"LB")
						and (ParseAnimationPartsColorBlendColorValue(child, partsColorValue.colors[2])))
					{
						continue;
					}
					if ((child.name() == U"RB")
						and (ParseAnimationPartsColorBlendColorValue(child, partsColorValue.colors[3])))
					{
						continue;
					}
				}
				out.attributeVariantValue = partsColorValue;
				return true;
			}
			case AttributeKind::Shader:
			{
				AttributeValueShader shaderValue;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{
					if (Utilities::TextToString(child, U"id", shaderValue.id))
					{
						continue;
					}
					if (Utilities::TextToFloat(child, U"param0", shaderValue.param[0]))
					{
						continue;
					}
					if (Utilities::TextToFloat(child, U"param1", shaderValue.param[1]))
					{
						continue;
					}
					if (Utilities::TextToFloat(child, U"param2", shaderValue.param[2]))
					{
						continue;
					}
					if (Utilities::TextToFloat(child, U"param3", shaderValue.param[3]))
					{
						continue;
					}
				}
				out.attributeVariantValue = shaderValue;
				return true;
			}
			case AttributeKind::Vertex:
			{
				AttributeValueVertexTranslate vertexTransValue;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{
					if (Utilities::TextToFloat2(child, U"LT", vertexTransValue.trans[0]))
					{
						continue;
					}
					if (Utilities::TextToFloat2(child, U"RT", vertexTransValue.trans[1]))
					{
						continue;
					}
					if (Utilities::TextToFloat2(child, U"LB", vertexTransValue.trans[2]))
					{
						continue;
					}
					if (Utilities::TextToFloat2(child, U"RB", vertexTransValue.trans[3]))
					{
						continue;
					}
				}
				out.attributeVariantValue = vertexTransValue;
				return true;
			}
			case AttributeKind::User:
			{
				AttributeValueUser userValue;
				int32 tmpInt;
				RectF tmpRect;
				Float2 tmpPoint;
				String tmpStr;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{
					if (Utilities::TextToInt32(child, U"integer", tmpInt))
					{
						userValue.integerOpt = tmpInt;
						continue;
					}
					if (Utilities::TextToRect(child, U"rect", tmpRect))
					{
						userValue.rectOpt = tmpRect;
						continue;
					}
					if (Utilities::TextToFloat2(child, U"point", tmpPoint))
					{
						userValue.pointOpt = tmpPoint;
						continue;
					}
					if (Utilities::TextToString(child, U"string", tmpStr))
					{
						userValue.stringOpt = tmpStr;
						continue;
					}
				}
				out.attributeVariantValue = userValue;
				return true;
			}
			case AttributeKind::Signal:
			{
				AttributeValueSignal signalValue;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{
					AttributeValueSignal::Command command;
					if (ParseAnimationSignalCommand(child, command))
					{
						signalValue.commands.emplace_back(command);
					}
				}
				out.attributeVariantValue = signalValue;
				return true;
			}
			case AttributeKind::InstanceParam:
			{
				AttributeValueInstance instanceValue;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{
					if (Utilities::TextToBool(child, U"infinity", instanceValue.isInfinity))
					{
						continue;
					}
					if (Utilities::TextToBool(child, U"reverse", instanceValue.isReverse))
					{
						continue;
					}
					if (Utilities::TextToBool(child, U"pingpong", instanceValue.isPingPong))
					{
						continue;
					}
					if (Utilities::TextToBool(child, U"independent", instanceValue.isIndependent))
					{
						continue;
					}
					if (Utilities::TextToInt32(child, U"loopNum", instanceValue.loopNum))
					{
						continue;
					}
					if (Utilities::TextToString(child, U"startLabel", instanceValue.startLabel))
					{
						continue;
					}
					if (Utilities::TextToInt32(child, U"startOffset", instanceValue.startOffset))
					{
						continue;
					}
					if (Utilities::TextToString(child, U"endLabel", instanceValue.endLabel))
					{
						continue;
					}
					if (Utilities::TextToInt32(child, U"endOffset", instanceValue.endOffset))
					{
						continue;
					}
					if (Utilities::TextToFloat(child, U"speed", instanceValue.speed))
					{
						continue;
					}
				}
				out.attributeVariantValue = instanceValue;
				return true;
			}
			case AttributeKind::Effect:
			{
				AttributeValueEffect effectValue;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{
					if (Utilities::TextToInt32(child, U"startTime", effectValue.startTime))
					{
						continue;
					}
					if (Utilities::TextToFloat(child, U"speed", effectValue.speed))
					{
						continue;
					}
					if (Utilities::TextToBool(child, U"independent", effectValue.isIndependent))
					{
						continue;
					}
				}
				out.attributeVariantValue = effectValue;
				return true;
			}
			case AttributeKind::Deform:
			{
				int32 vertexSize = 0;
				String vertexChangeList = U"";
				AttributeValueDeform deformValue;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{
					if (Utilities::TextToInt32(child, U"vsize", vertexSize))
					{
						continue;
					}
					if (Utilities::TextToString(child, U"vchg", vertexChangeList))
					{
						continue;
					}
				}
				deformValue.verticesChanges.resize(static_cast<size_t>(vertexSize), Float2::Zero());
				const auto& lines = vertexChangeList.split(U' ');
				if (1 < lines.size())
				{
					const size_t infoSize = Parse<size_t>(lines[0]);
					for (size_t i = 0; i < infoSize; i++)
					{
						const size_t lineIndex = 1 + i * 3;
						const int32 index = Parse<int32>(lines[lineIndex + 0]);
						const float x = Parse<float>(lines[lineIndex + 1]);
						const float y = Parse<float>(lines[lineIndex + 2]);
						if (0 <= index and index < vertexSize)
						{
							deformValue.verticesChanges[index].set(x, y);
						}
					}
				}
				out.attributeVariantValue = deformValue;
				return true;
			}
			case AttributeKind::Audio:
			{
				// TODO:
				AttributeValueAudio audioValue;
				out.attributeVariantValue = audioValue;
				return true;
			}
			case AttributeKind::TextureChange:
			{
				AttributeValueTextureChange textureChangeValue;
				for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
				{
					if (Utilities::TextToString(child, U"TextureName", textureChangeValue.textureName))
					{
						continue;
					}
				}
				out.attributeVariantValue = textureChangeValue;
				return true;
			}

			case AttributeKind::VertexColor: // [VCOL]非対応
			case AttributeKind::Invalid:
			default:
				break;
			}

			// 未対応 or 非対応
			DebugLog::Print(DebugLog::LogType::Error, U"非対応または未対応のパラメータです。");
			return false;
		}

		//================================================================================
		bool ParseAnimationKeyFrames(const XMLElement& element, AttributeKind kind, Array<AnimationKeyFrame>& out)
		{
			if (element.name() != U"key")
			{
				return false;
			}
			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation keyframes ---");

			AnimationKeyFrame keyFrame;

			// 補完できない,しないものは指定なしもある。
			Utilities::AttributeToEnum(element, U"ipType", InterpolationTypeTable, keyFrame.interpolationType);

			if (not(Utilities::AttributeToInt32(element, U"time", keyFrame.frame)))
			{
				// フレーム指定なしはおかしい。
				DebugLog::Print(DebugLog::LogType::Error, U"キーフレームデータのフレームがありません。");
				return false;
			}

			Utilities::AttributeToFloat(element, U"easingRate", keyFrame.easingRate);

			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseAnimationCurve(child, keyFrame.curve))
				{
					continue;
				}
				if (ParseAnimationKeyFrame(child, kind, keyFrame))
				{
					continue;
				}
			}
			out.emplace_back(keyFrame);
			return true;
		}

		//================================================================================
		bool ParseAnimationAttribute(const XMLElement& element, AnimationAttribute& out)
		{
			if (element.name() != U"attribute")
			{
				return false;
			}

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation attribute ---");
			if (not(Utilities::AttributeToEnum(element, U"tag", AttributeKindTable, out.kind)))
			{
				// アトリビュートのタグが見つからない場合は未対応項目がある。
				DebugLog::Print(DebugLog::LogType::Notice, U"未対応のアトリビュートです。");
				return false;
			}

			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseAnimationKeyFrames(child, out.kind, out.keyFrames))
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

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation attributes ---");
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

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation part ---");
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

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation parts ---");
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

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation ---");
			AnimationSetting overrideSetting;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToString(child, U"name", out.name))
				{
					continue;
				}
				if (ParseAnimationSetting(child, overrideSetting))
				{
					out.overrideSettingOpt = overrideSetting;
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

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation list ---");
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
			DebugLog::Print(DebugLog::LogType::Verbose, U"--- animation pack ---");
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
