
template< class T, size_t N >
inline bool TextToEnum(const XMLElement& element, StringView name, const std::array<std::pair<const char32_t*, T>, N>& table, T& out)
{
	if (element.name() != name)
	{
		return false;
	}

	DebugLog::Print(DebugLog::LogType::Verbose, U"--- text to enum -------------");
	DebugLog::Print(DebugLog::LogType::Verbose, U"name:{}"_fmt(name));
	DebugLog::Print(DebugLog::LogType::Verbose, U"text:{}"_fmt(element.text()));

	for (const auto& it : table)
	{
		if (element.text() == it.first)
		{
			out = it.second;
			DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(FromEnum(out)));
			return true;
		}
	}

	DebugLog::Print(DebugLog::LogType::Notice, U"未反映のパラメータを検知しました。");
	return false;
}

//================================================================================
template< class T, size_t N >
inline bool AttributeToEnum(const XMLElement& element, StringView key, const std::array<std::pair<const char32_t*, T>, N>& table, T& out)
{
	auto attribute = element.attribute(String(key));

	DebugLog::Print(DebugLog::LogType::Verbose, U"--- attribute to enum --------");
	if (not(attribute))
	{
		DebugLog::Print(DebugLog::LogType::Verbose, U"キーが見つかりませんでした。 key:{}"_fmt(key));
		return false;
	}

	DebugLog::Print(DebugLog::LogType::Verbose, U"key :{}"_fmt(key));
	DebugLog::Print(DebugLog::LogType::Verbose, U"val :{}"_fmt(attribute.value()));

	for (const auto& it : table)
	{
		if (attribute.value() == it.first)
		{
			out = it.second;
			DebugLog::Print(DebugLog::LogType::Verbose, U"out :{}"_fmt(FromEnum(out)));
			return true;
		}
	}

	DebugLog::Print(DebugLog::LogType::Notice, U"未反映のパラメータを検知しました。");
	return false;
}
