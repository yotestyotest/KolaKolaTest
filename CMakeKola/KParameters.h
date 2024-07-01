#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "KConsole.h"

class KParameters
{
	inline static auto database_filename = "KParameters.txt";

public:
	struct Data
	{
		double hourly_wage{};
		double supplement_in_weekend{};
		double supplement_in_evening{};
		double tax{};

		bool valid() const { return hourly_wage >= 0.01 && supplement_in_weekend >= 0. && supplement_in_evening >= 0. && tax >= 0. && tax <= 100.; }

		std::string serialize() const
		{
			std::ostringstream ss;
			ss << hourly_wage << " " << supplement_in_weekend << " " << supplement_in_evening << " " << tax;
			return ss.str();
		}

		bool deserialize(std::string s)
		{
			Data d;
			std::stringstream is(s);
			if (!(is >> d.hourly_wage) || !(is >> d.supplement_in_weekend) || !(is >> d.supplement_in_evening) || !(is >> d.tax) || !d.valid())
				return false;

			*this = d;
			return true;
		}
	};

	const Data& read() const { return m_data; }

	bool valid() const { return m_data.valid(); }

	void set_example()
	{
		m_data = { 145.5, 23.5, 15.5, 25 };
	}

	std::string print_full() const
	{
		std::ostringstream ss;
		ss << "Hourly wage: " << m_data.hourly_wage
			<< " Weekend suppl: " << m_data.supplement_in_weekend
			<< " Evening suppl: " << m_data.supplement_in_evening
			<< " Tax: " << m_data.tax << "%";
		return ss.str();
	}

	std::string print_short() const
	{
		if (!valid())
			return "<Parameters not set>";

		std::ostringstream ss;
		ss << "$" << m_data.hourly_wage
			<< "/h Wknd/Evng: " << m_data.supplement_in_weekend
			<< "/" << m_data.supplement_in_evening
			<< " Tax: " << m_data.tax << "%";
		return ss.str();
	}

	void save() const
	{
		try
		{
			{
				std::ofstream fs(database_filename, std::ios::binary);
				fs << m_data.serialize();
			}
			std::cout << " Saved " << std::filesystem::file_size(database_filename) << " bytes into " << database_filename << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << " Failed to save " << database_filename << ": " << e.what() << std::endl;
		}
	}

	void load()
	{
		try
		{
			std::ifstream fs(database_filename, std::ios::binary);
			if (!fs)
			{
				std::cout << "No data in " << database_filename << "\n";
				return;
			}
			std::stringstream buffer;
			buffer << fs.rdbuf();
			if(!m_data.deserialize(buffer.str()))
				std::cerr << " Failed to interpret " << database_filename << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << " Failed to load " << database_filename << ": " << e.what() << std::endl;
		}
	}

	void edit();

private:
	Data m_data;
};

inline std::ostream& operator<<(std::ostream& os, const KParameters& p) 
{
	os << p.print_full();
	return os;
}

inline void KParameters::edit()
{
	Choices editMenu = { {'A', "Accept"}, 
		{'H', "$Hourly"}, {'W', "$Weekend"}, {'E', "$Evening"}, {'T', "%Tax"}, 
		{'D', "Defaults"}, {'S', "Save"}, {'L', "Load"} };
	while (true)
	{
		char choice = UserChoice(print_full(), editMenu);
		switch (choice)
		{
		case 'A': return;
		case 'D': set_example(); break;
		case 'H': DoubleInput(m_data.hourly_wage, "Hourly wage", 0.01); break;
		case 'W': DoubleInput(m_data.supplement_in_weekend, "Weekend suppl", 0.); break;
		case 'E': DoubleInput(m_data.supplement_in_evening, "Evening suppl", 0.); break;
		case 'T': DoubleInput(m_data.tax, "Tax, %", 0, 100); break;
		case 'L': load(); break;
		case 'S': save(); break;
		default: throw std::invalid_argument("Invalid menu item");
		}
	}
}
