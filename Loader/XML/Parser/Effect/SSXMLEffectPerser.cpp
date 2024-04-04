#include "SSXMLEffectPerser.hpp"
#include "../../../../Common/SSOutputDebugLog.hpp"
#include "../../SSXMLEnumTable.hpp"
#include "../SSXMLParserUtilities.hpp"

namespace s3d::SpriteStudio::XMLParser
{
	namespace
	{
		//================================================================================
		bool ParseRangeValueInt32(const XMLElement& element, StringView name, RangeValue<int32>& out)
		{
			if (element.name() != name)
			{
				return false;
			}
			int32 value = 0, subValue = 0;
			if (not(Utilities::AttributeToInt32(element, U"value", value)))
			{
				return false;
			}
			if (not(Utilities::AttributeToInt32(element, U"subvalue", subValue)))
			{
				return false;
			}
			out.setMinMax(value, subValue);
			return true;
		}

		//================================================================================
		bool ParseRangeValueFloat(const XMLElement& element, StringView name, RangeValue<float>& out)
		{
			if (element.name() != name)
			{
				return false;
			}
			float value = 0.0f, subValue = 0.0f;
			if (not(Utilities::AttributeToFloat(element, U"value", value)))
			{
				return false;
			}
			if (not(Utilities::AttributeToFloat(element, U"subvalue", subValue)))
			{
				return false;
			}
			out.setMinMax(value, subValue);
			return true;
		}

		//================================================================================
		bool ParseRangeValueColor(const XMLElement& element, StringView name, RangeValue<Color>& out)
		{
			if (element.name() != name)
			{
				return false;
			}
			Color value = Palette::White, subValue = Palette::White;
			if (not(Utilities::AttributeToColor(element, U"value", value)))
			{
				return false;
			}
			if (not(Utilities::AttributeToColor(element, U"subvalue", subValue)))
			{
				return false;
			}
			out.setMinMax(value, subValue);
			return true;
		}

		//================================================================================
		bool ParseEffectElementBasic(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementBasic effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToInt32(child, U"maximumParticle", effectElement.maximumParticle))
				{
					continue;
				}
				if (ParseRangeValueFloat(child, U"speed", effectElement.speed))
				{
					continue;
				}
				if (ParseRangeValueInt32(child, U"lifespan", effectElement.lifeSpan))
				{
					continue;
				}
				if (Utilities::TextToFloat(child, U"angle", effectElement.angle))
				{
					continue;
				}
				if (Utilities::TextToFloat(child, U"angleVariance", effectElement.angleVariance))
				{
					continue;
				}
				if (Utilities::TextToInt32(child, U"interval", effectElement.interval))
				{
					continue;
				}
				if (Utilities::TextToInt32(child, U"lifetime", effectElement.lifeTime))
				{
					continue;
				}
				if (Utilities::TextToInt32(child, U"attimeCreate", effectElement.atTimeCreate))
				{
					continue;
				}
				if (Utilities::TextToInt32(child, U"priority", effectElement.priority))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementRandomSeedChange(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementRandomSeedChange effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToInt32(child, U"Seed", effectElement.seed))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementDelay(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementDelay effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToInt32(child, U"DelayTime", effectElement.time))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementGravity(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementGravity effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToFloat2(child, U"Gravity", effectElement.gravity))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementInitializePosition(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementPosition effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseRangeValueFloat(child, U"OffsetX", effectElement.offsetX))
				{
					continue;
				}
				if (ParseRangeValueFloat(child, U"OffsetY", effectElement.offsetY))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementInitializeRotation(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementRotation effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseRangeValueFloat(child, U"Rotation", effectElement.rotation))
				{
					continue;
				}
				if (ParseRangeValueFloat(child, U"RotationAdd", effectElement.rotationAdd))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementTransformRotation(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementTransformRotation effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToFloat(child, U"RotationFactor", effectElement.rotationFactor))
				{
					continue;
				}
				if (Utilities::TextToFloat(child, U"EndLifeTimePer", effectElement.endLifeTimePercent))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementTransformSpeed(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementTransformSpeed effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseRangeValueFloat(child, U"Speed", effectElement.speed))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementTangentialAcceleration(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementTangentialAcceleration effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseRangeValueFloat(child, U"Acceleration", effectElement.acceleration))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementInitializeColor(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementInitializeColor effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseRangeValueColor(child, U"Color", effectElement.color))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementTransformColor(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementTransformColor effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseRangeValueColor(child, U"Color", effectElement.color))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementAlphaFade(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementAlphaFade effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseRangeValueFloat(child, U"disprange", effectElement.dispRange))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementInitializeSize(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementInitializeSize effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseRangeValueFloat(child, U"SizeX", effectElement.sizeX))
				{
					continue;
				}
				if (ParseRangeValueFloat(child, U"SizeY", effectElement.sizeY))
				{
					continue;
				}
				if (ParseRangeValueFloat(child, U"ScaleFactor", effectElement.scaleFactor))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementTransformSize(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementTransformSize effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseRangeValueFloat(child, U"SizeX", effectElement.sizeX))
				{
					continue;
				}
				if (ParseRangeValueFloat(child, U"SizeY", effectElement.sizeY))
				{
					continue;
				}
				if (ParseRangeValueFloat(child, U"ScaleFactor", effectElement.scaleFactor))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementPointGravity(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementPointGravity effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToFloat2(child, U"Position", effectElement.position))
				{
					continue;
				}
				if (Utilities::TextToFloat(child, U"Power", effectElement.power))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementTurnToDirectionEnabled(const XMLElement& element, EffectElementVariant& out)
		{
			EffectElementTurnToDirectionEnabled effectElement;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToFloat(child, U"Rotation", effectElement.rotation))
				{
					continue;
				}
			}
			out = effectElement;
			return true;
		}

		//================================================================================
		bool ParseEffectElementInfiniteEmitEnabled([[maybe_unused]] const XMLElement& element, EffectElementVariant& out)
		{
			out = EffectElementInfiniteEmitEnabled();
			return true;
		}

		//================================================================================
		bool ParseEffectElement(const XMLElement& element, StringView name, EffectElementVariant& out)
		{
			if (name.isEmpty())
			{
				return false;
			}
			if (name == U"Basic")
			{
				return ParseEffectElementBasic(element, out);
			}
			if (name == U"OverWriteSeed")
			{
				return ParseEffectElementRandomSeedChange(element, out);
			}
			if (name == U"Delay")
			{
				return ParseEffectElementDelay(element, out);
			}
			if (name == U"Gravity")
			{
				return ParseEffectElementGravity(element, out);
			}
			if (name == U"init_position")
			{
				return ParseEffectElementInitializePosition(element, out);
			}
			if (name == U"init_rotation")
			{
				return ParseEffectElementInitializeRotation(element, out);
			}
			if (name == U"trans_rotation")
			{
				return ParseEffectElementTransformRotation(element, out);
			}
			if (name == U"trans_speed")
			{
				return ParseEffectElementTransformSpeed(element, out);
			}
			if (name == U"add_tangentiala")
			{
				return ParseEffectElementTangentialAcceleration(element, out);
			}
			if (name == U"init_vertexcolor")
			{
				return ParseEffectElementInitializeColor(element, out);
			}
			if (name == U"trans_vertexcolor")
			{
				return ParseEffectElementTransformColor(element, out);
			}
			if (name == U"trans_colorfade")
			{
				return ParseEffectElementAlphaFade(element, out);
			}
			if (name == U"init_size")
			{
				return ParseEffectElementInitializeSize(element, out);
			}
			if (name == U"trans_size")
			{
				return ParseEffectElementTransformSize(element, out);
			}
			if (name == U"add_pointgravity")
			{
				return ParseEffectElementPointGravity(element, out);
			}
			if (name == U"TurnToDirection")
			{
				return ParseEffectElementTurnToDirectionEnabled(element, out);
			}
			if (name == U"InfiniteEmit")
			{
				return ParseEffectElementInfiniteEmitEnabled(element, out);
			}
			// 該当なし？
			return false;
		}

		//================================================================================
		bool ParseEffectElements(const XMLElement& element, Array<EffectElementVariant>& out)
		{
			if (element.name() != U"list")
			{
				return false;
			}

			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				String name = U"";
				if (not(Utilities::AttributeToString(child, U"name", name)))
				{
					throw Error{ U"EffectElementの名前が無いので解析できませんでした。" };
				}
				EffectElementVariant elementVariant;
				if (ParseEffectElement(child, name, elementVariant))
				{
					out.emplace_back(elementVariant);
				}
			}
			return true;
		}

		//================================================================================
		bool ParseEffectBehavior(const XMLElement& element, EffectBehavior& out)
		{
			if (element.name() != U"behavior")
			{
				return false;
			}
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToString(child, U"CellName", out.cellName))
				{
					continue;
				}
				if (Utilities::TextToString(child, U"CellMapName", out.cellmapName))
				{
					continue;
				}
				if (Utilities::TextToEnum(child, U"BlendType", BlendTypeTable, out.blendType))
				{
					continue;
				}
				if (ParseEffectElements(child, out.elements))
				{
					continue;
				}
			}
			return true;
		}

		//================================================================================
		bool ParseEffectNode(const XMLElement& element, EffectNode& out)
		{
			if (element.name() != U"node")
			{
				return false;
			}
			EffectBehavior behavior;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToInt32(child, U"arrayIndex", out.arrayIndex))
				{
					continue;
				}
				if (Utilities::TextToInt32(child, U"parentIndex", out.parentIndex))
				{
					continue;
				}
				if (Utilities::TextToEnum(child, U"type", EffectNodeTable, out.type))
				{
					continue;
				}
				if (Utilities::TextToBool(child, U"visible", out.isVisible))
				{
					continue;
				}
				if (ParseEffectBehavior(child, behavior))
				{
					out.behaviorOpt = behavior;
					continue;
				}
			}
			return true;
		}

		//================================================================================
		bool ParseEffectNodes(const XMLElement& element, Array<EffectNode>& out)
		{
			if (element.name() != U"nodeList")
			{
				return false;
			}
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				EffectNode effectNode;
				if (ParseEffectNode(child, effectNode))
				{
					out.emplace_back(effectNode);
				}
			}
			return true;
		}

		//================================================================================
		bool ParseEffectModel(const XMLElement& element, EffectModel& out)
		{
			if (element.name() != U"effectData")
			{
				return false;
			}
			int32 lockRandomSeed = 0;
			bool isLockRandomSeed = true;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToInt32(child, U"lockRandSeed", lockRandomSeed))
				{
					out.lockRandomSeedOpt = lockRandomSeed;
					continue;
				}
				if (Utilities::TextToBool(child, U"isLockRandSeed", isLockRandomSeed))
				{
					continue;
				}
				if (Utilities::TextToInt32(child, U"fps", out.fps))
				{
					continue;
				}
				if (Utilities::TextToInt32(child, U"layoutScaleX", out.layoutScaleX))
				{
					continue;
				}
				if (Utilities::TextToInt32(child, U"layoutScaleY", out.layoutScaleY))
				{
					continue;
				}
				if (ParseEffectNodes(child, out.nodes))
				{
					continue;
				}
			}
			// ランダムシードロックしないのであれば消しておく。
			if (not(isLockRandomSeed))
			{
				out.lockRandomSeedOpt = none;
			}
			// 初期値のままであれば規定値を放り込む
			if (out.layoutScaleX == 0)
			{
				out.layoutScaleX = 100;
			}
			if (out.layoutScaleY == 0)
			{
				out.layoutScaleY = 100;
			}

			// ツリーの構築
			const size_t nodeCount = out.nodes.size();
			if (0 < nodeCount)
			{
				for (size_t i = 1; i < nodeCount; i++)
				{
					const int32 parentIndex = out.nodes[i].parentIndex;
					if (0 <= parentIndex)
					{
						out.nodes[parentIndex].linkToEndChild(&(out.nodes[i]));
					}
				}
			}
			return true;
		}

		//================================================================================
		bool ParseEffect(const XMLElement& element, Effect& out)
		{
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToString(child, U"name", out.name))
				{
					continue;
				}
				if (ParseEffectModel(child, out.model))
				{
					continue;
				}
			}
			out.model.name = out.name;
			return true;
		}

	}

	//================================================================================
	bool EffectParser(const XMLElement& element, Effect& out)
	{
		try
		{
			return ParseEffect(element, out);
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
