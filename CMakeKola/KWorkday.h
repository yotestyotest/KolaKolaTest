#pragma once
#include "KDate.h"
#include "KHour.h"
#include "KConsole.h"

class KWorkday
{
public:
	KWorkday() = default;

	KWorkday(KDate d, KHour wstart, KHour wend)
		: m_data{ d, wstart, wend }
	{ }

	// YYYY-MM-DD H1 H2
	KWorkday(std::string date_start_end_spaced)
	{
		std::stringstream is(date_start_end_spaced);
		std::string d;
		double h1{ -1 }, h2{ -2 };

		if (!(is >> d) || !(is >> h1) || !(is >> h2))
		{
			//std::cout << "unable to parse 3 parameters\n";
			return;
		}

		m_data = Data{ KDate::fromString(d), h1, h2 };
	}

	struct Data
	{
		KDate date;
		KHour start;
		KHour end;

		// comparison for sorting
		bool operator<(const Data& other)
		{
			if (date.str() != other.date.str())
				return date.str() < other.date.str();
			if (start != other.start)
				return start < other.start;
			return end < other.end;
		}
	};

	const Data& read() const { return m_data; }

	std::string datestr() const { return m_data.date.str(); }

	bool valid() const
	{
		return m_data.date.valid() && m_data.start.valid() && m_data.end.valid() && m_data.start < m_data.end;
	}

	// how much time worked between 07:00 and 17:00
	double working_hours_day() const
	{
		if (!valid())
			return {};
		double h1 = m_data.start <= 7. ? KHour(7.) : m_data.start;
		double h2 = m_data.end >= 17. ? KHour(17.) : m_data.end;
		if (h1 < h2)
			return h2 - h1;
		return 0;
	}

	// how much time worked between 00:00 and 07:00
	double working_hours_night() const
	{
		if (!valid())
			return {};
		double h1 = m_data.start;
		double h2 = m_data.end >= 7. ? KHour(7.) : m_data.end;
		if (h1 < h2)
			return h2 - h1;
		return 0;
	}

	// how much time worked between 17:00 and 24:00
	double working_hours_evening() const
	{
		if (!valid())
			return {};
		double h1 = m_data.start <= 17. ? KHour(17.) : m_data.start;
		double h2 = m_data.end;
		if (h1 < h2)
			return h2 - h1;
		return 0;
	}

	std::string print_full() const
	{
		std::ostringstream ss;
		if (!valid())
			ss << "Invalid:";
		ss << m_data.date.str() << " " << m_data.start.str() << ".." << m_data.end.str();
		return ss.str();
	}

	void edit()
	{
		Choices editMenu = { {'A', "Accept workday"}, {'D', "Date"}, {'S', "Start work"}, {'E', "End work"}, {'R', "Row record"} };
		while (true)
		{
			char choice = UserChoice(print_full(), editMenu);
			switch (choice)
			{
			case 'A': return;
			case 'R': input_record(); break;
			case 'D': input_date();	break;
			case 'S':
			{
				auto def = m_data.start.valid() ? m_data.start : KHour(7.5);
				m_data.start = DoubleInput("Start hour", def, 0, 24);
				break;
			}
			case 'E':
			{
				auto def = m_data.end.valid() ? m_data.end : KHour(17.);
				m_data.end = DoubleInput("End hour", def, 0, 24);
				break;
			}
			default: throw std::invalid_argument("Invalid menu item");
			}
		}
	}

	void input_date()
	{
		while (true)
		{
			auto inp = StringInput("Date YYYY-MM-DD", "", 10, 10);
			if (inp.empty())
				return; // user just pressed enter, return back to menu
			auto newdate = KDate::fromString(inp);
			if (newdate.valid())
			{
				m_data.date = newdate;
				return;
			}
			std::cout << " Invalid date, please enter valid date or press Enter to return\n";
		}
	}

	// in case if user want to input all values at once
	void input_record()
	{
		while (true)
		{
			std::cout << "Enter the full workday record, example: 2024-02-28 07 17\nYYYY-MM-DD H1 H2: ";
			std::string inpline;
			std::getline(std::cin, inpline);
			if (inpline.empty())
				return; // user just pressed Enter

			KWorkday newvalue(inpline);
			if (newvalue.valid())
			{
				*this = newvalue; // todo: swap for performance
				return;
			}

			std::cout << "unable to parse 3 parameters - incorrect" << newvalue.print_full() << "\n";
		}
	}

	// comparison for sorting
	inline bool operator<(const KWorkday& other)
	{
		return m_data < other.m_data;
	}

private:
	Data m_data;
};
