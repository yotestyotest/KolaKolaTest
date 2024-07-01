#pragma once

#include <sstream>
#include <string>
#include <iomanip> // for std::setw and std::setfill TODO: use std::print in new C++

class KHour
{
	double m_time{ -1 };
public:

	KHour(double h = -1)
		: m_time(h)
	{ }

	bool valid() const { return 0. <= m_time && m_time < 24.; }

	std::string str() const
	{
		if (!valid())
			return "N/A";
		std::ostringstream os;
		const auto hours = static_cast<int>(m_time);
		const auto minutes = static_cast<int>((m_time - hours) * 60.);
		os << std::setw(2) << std::setfill('0') << hours << ":"
			<< std::setw(2) << std::setfill('0') << minutes;
		return os.str();
	}

	static KHour fromString(std::string s)
	{
		double val{ -1 };
		if (!s.empty() && (std::stringstream(s) >> val))
			return KHour(val);
		return KHour();
	}

	operator double() const { return m_time; }
};
