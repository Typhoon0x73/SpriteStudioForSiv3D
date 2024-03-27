#include "SSXMLCellmapParser.hpp"
#include "../SSXMLParserUtilities.hpp"
#include "../../../../Common/SSOutputDebugLog.hpp"
#include "../../../../Common/SSTypeDefine.hpp"
#include "../../SSXMLEnumTable.hpp"
#include "../../../../Data/Project/Setting/SSProjectSetting.hpp"

namespace s3d::SpriteStudio::XMLParser
{
	namespace
	{
		//================================================================================
		bool ParseSettings(const XMLElement& element, ProjectSetting& out)
		{
			if (element.name() != U"settings")
			{
				return false;
			}

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- project settings ---");
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToEnum(child, U"wrapMode", TextureAddressModeTable, out.addressMode))
				{
					continue;
				}
				if (Utilities::TextToEnum(child, U"filterMode", TextureFilterTable, out.filterMode))
				{
					continue;
				}
			}
			return true;
		}

		//================================================================================
		bool ParseCellMeshInfoPointList(const XMLElement& element, Array<Float2>& out)
		{
			if (element.name() != U"meshPointList")
			{
				return false;
			}

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- cell mesh info point list ---");
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				Float2 meshPoint;
				if (Utilities::TextToFloat2(child, U"value", meshPoint))
				{
					out.emplace_back(meshPoint);
				}
			}
			return true;
		}

		//================================================================================
		bool ParseCellMeshInfoTriangleList(const XMLElement& element, Array<TriangleIndex>& out)
		{
			if (element.name() != U"meshTriList")
			{
				return false;
			}

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- cell mesh info triangle list ---");
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				TriangleIndex triangleIndex;
				if (Utilities::TextToTriangleIndex(child, U"value", triangleIndex))
				{
					out.emplace_back(triangleIndex);
				}
			}
			return true;
		}

		//================================================================================
		bool ParseCellMeshInfo(const XMLElement& element, CellMeshInfo& out)
		{
			DebugLog::Print(DebugLog::LogType::Verbose, U"--- cell mesh info ---");
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (ParseCellMeshInfoPointList(child, out.points))
				{
					continue;
				}
				if (ParseCellMeshInfoTriangleList(child, out.triangles))
				{
					continue;
				}
			}
			return true;
		}

		//================================================================================
		bool ParseCell(const XMLElement& element, Cell& out)
		{
			if (element.name() != U"cell")
			{
				return false;
			}

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- cell ---");
			bool isMesh = false;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToString(child, U"name", out.name))
				{
					continue;
				}
				if (Utilities::TextToPoint(child, U"pos", out.rect.pos))
				{
					continue;
				}
				if (Utilities::TextToSize(child, U"size", out.rect.size))
				{
					continue;
				}
				if (Utilities::TextToFloat2(child, U"size", out.pivot))
				{
					continue;
				}
				if (Utilities::TextToBool(child, U"rotated", out.isRotated))
				{
					continue;
				}
				if (Utilities::TextToBool(child, U"ismesh", isMesh))
				{
					continue;
				}
			}
			if (isMesh)
			{
				CellMeshInfo meshInfo;
				if (ParseCellMeshInfo(element, meshInfo))
				{
					out.meshInfoOpt = meshInfo;
				}
			}

			return true;
		}

		//================================================================================
		bool ParseCells(const XMLElement& element, Array<Cell>& out)
		{
			if (element.name() != U"cells")
			{
				return false;
			}

			DebugLog::Print(DebugLog::LogType::Verbose, U"--- cells ---");
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				Cell cell;
				if (ParseCell(child, cell))
				{
					out.emplace_back(cell);
				}
			}

			return true;
		}

		//================================================================================
		bool ParseCellmap(const XMLElement& element, Cellmap& out)
		{
			DebugLog::Print(DebugLog::LogType::Verbose, U"--- cellmap ---");
			ProjectSetting overrideSetting;
			for (auto child = element.firstChild(); not(child.isNull()); child = child.nextSibling())
			{
				if (Utilities::TextToString(child, U"name", out.name))
				{
					continue;
				}
				if (Utilities::TextToString(child, U"imagePath", out.imagePath))
				{
					continue;
				}
				if (Utilities::TextToSize(child, U"pixelSize", out.pixelSize))
				{
					continue;
				}
				if (Utilities::TextToSize(child, U"pixelSize", out.pixelSize))
				{
					continue;
				}
				if (ParseSettings(child, overrideSetting))
				{
					out.overrideSettingOpt = overrideSetting;
					continue;
				}
				if (ParseCells(child, out.cells))
				{
					continue;
				}
			}
			return true;
		}
	}

	//================================================================================
	bool CellmapParser(const XMLElement& element, Cellmap& out)
	{
		try
		{
			return ParseCellmap(element, out);
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
