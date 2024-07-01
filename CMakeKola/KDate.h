#pragma once

#include <string>
#include <regex>
#include <iostream>
#include <ctime>
#include <chrono>

class KDate
{
	inline const static std::regex looksLikeISO{ R"(\d{4}-\d{2}-\d{2})" };
	std::string m_date;
	std::tm m_tm{};
	int m_dayofweek{ 1 };

	void check_weekend()
	{
		std::time_t time = std::mktime(&m_tm);
		std::tm local_tm = {};
		errno_t err = localtime_s(&local_tm, &time);
		if (err != 0)
			throw std::invalid_argument("Failed to convert time to local time");

		m_dayofweek = local_tm.tm_wday;  // Sunday = 0, Monday = 1, ..., Saturday = 6
	}

public:
	bool valid() const { return !m_date.empty(); }
	
	bool weekend() const { return m_dayofweek == 0 || m_dayofweek == 6; }

	const std::string& str() const { return m_date; }

	static KDate fromString(std::string s)
	{
		try
		{
			if (!std::regex_match(s, looksLikeISO))
				return {};

			std::istringstream ss(s);
			std::tm tm{};
			ss >> std::get_time(&tm, "%Y-%m-%d");
			if (ss.fail())
				return {};

			KDate res;
			res.m_date = s;
			res.m_tm = tm;
			res.check_weekend();
			return res;
		}
		catch (const std::exception& e) 
		{
			std::cerr << "KDate::fromString error: " << e.what() << '\n';
		}
		return {};
	}
};
