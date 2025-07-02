#include "SSOutputDebugLog.hpp"
#include <bitset>

namespace s3d::SpriteStudio::DebugLog
{
	#ifdef SIV3D_SS_USE_LOGGER
	namespace
	{
		TextWriter* pExportWriter = nullptr;
		ExportBuffer ExportBufferInstance = Logger;
		std::bitset<8> ExportTypeFlags = 0b0000'1111;
	}
	#endif // SIV3D_SS_USE_LOGGER

	//================================================================================
	void ChangeBuffer([[maybe_unused]] ExportBuffer buffer)
	{
		#ifdef SIV3D_SS_USE_LOGGER
		ExportBufferInstance = buffer;
		#endif // SIV3D_SS_USE_LOGGER
	}

	//================================================================================
	void ExportWriter([[maybe_unused]] TextWriter* pWriter)
	{
		#ifdef SIV3D_SS_USE_LOGGER
		pExportWriter = pWriter;
		#endif // SIV3D_SS_USE_LOGGER
	}

	//================================================================================
	void SetExportLog([[maybe_unused]] LogType type, [[maybe_unused]] LogEnable enable)
	{
		#ifdef SIV3D_SS_USE_LOGGER
		ExportTypeFlags.set(FromEnum(type), enable.getBool());
		#endif // SIV3D_SS_USE_LOGGER
	}

	//================================================================================
	void Print([[maybe_unused]] LogType type, [[maybe_unused]] StringView message)
	{
		#ifdef SIV3D_SS_USE_LOGGER

		if (not(ExportTypeFlags.test(FromEnum(type))))
		{
			return;
		}

		String typeStr = U"";
		switch (type)
		{
		case LogType::Warning:
		{
			typeStr = U"warning";
			break;
		}
		case LogType::Error:
		{
			typeStr = U"error  ";
			break;
		}
		case LogType::Notice:
		{
			typeStr = U"notice ";
			break;
		}
		case LogType::Info:
		{
			typeStr = U"info   ";
			break;
		}
		case LogType::Verbose:
		{
			typeStr = U"verbose";
			break;
		}
		default:
			break;
		}
		const String log = U"[SpriteStudioSDK][{}] {}"_fmt(typeStr, message);
		std::visit(
			[&](auto& buffer)
			{
				buffer << log;
			}
		, ExportBufferInstance
		);
		if (pExportWriter != nullptr)
		{
			(*pExportWriter) << log;
		}
		
		#endif // SIV3D_SS_USE_LOGGER
	}
}
