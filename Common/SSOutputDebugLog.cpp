#include "SSOutputDebugLog.hpp"

namespace s3d::SpriteStudio::DebugLog
{
	void Print([[maybe_unused]] LogType type, [[maybe_unused]] StringView message)
	{
		#ifdef SIV3D_SS_USE_LOGGER
		String typeStr = U"";
		switch (type)
		{
		case LogType::Werning:
		{
			typeStr = U"werning";
			break;
		}
		case LogType::Error:
		{
			typeStr = U"error";
			break;
		}
		case LogType::Notice:
		{
			typeStr = U"notice";
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
		Logger << U"[SpriteStudioSDK][{}] "_fmt(typeStr) << message;
		#endif // SIV3D_SS_USE_LOGGER
	}
}
