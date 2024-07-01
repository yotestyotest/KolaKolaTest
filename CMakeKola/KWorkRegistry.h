#pragma once

#include "KWorkday.h"
#include <fstream>
#include <filesystem>

class KWorkRegistry
{
	inline static auto database_filename = "KWorkRegistry.txt";
	std::vector<KWorkday> m_work;
public:
	
	const std::vector<KWorkday>& read() const { return m_work; }

	bool empty() const { return m_work.empty(); }

	bool valid() const 
	{
		// TODO: scan registry for logic errors
		// - invalid dates
		// - overlapping work records
		// - etc
		return !empty();
	}

	void add(KWorkday d) { m_work.emplace_back(d); sort(); }

	void clear() { m_work.clear(); }

	void sort() { std::sort(m_work.begin(), m_work.end()); }

	Choices edit_menu() const
	{
		if (empty())
			return { {'A', "Accept"}, {'I', "Input new"}, {'L', "Load"} };
		else
			return { {'A', "Accept"}, {'I', "Input new"}, { 'C', "Clear" }, { 'E', "Edit last" }, { '#', "Edit(#)"}, { 'P', "Print" }, {'L', "Load"}, {'S', "Save"} };
	}

	void edit()
	{
		while (true)
		{
			char choice = UserChoice(print_short(), edit_menu());
			switch (choice)
			{
			case 'A': return;
			case 'I': add_edit(); break;
			case 'C': clear(); break;
			case 'E': assert(!empty()); m_work.rbegin()->edit(); break;
			case 'P': std::cout << "\n" << print_full(); break;
			case '#': std::cout << "\n" << print_full(); edit_by_input_index(); break;
			case 'L': load(); break;
			case 'S': save(); break;
			default: throw std::invalid_argument("Invalid menu item");
			}
		}
	}

	void edit_by_input_index()
	{
		if (empty())
			throw std::logic_error("edit_by_input_index should not be called on empty schedule");
		std::ostringstream ss;
		ss << "Edit workday index 1.." << m_work.size() << " or date " << m_work.at(0).datestr() << "...";
		auto sidx = StringInput("Index of workday or date", "");
		if (sidx.empty())
			return;
		int idx = std::atoi(sidx.c_str());
		if (1 <= idx && idx <= m_work.size())
		{
			m_work[idx - 1].edit();
			return;
		}
		// maybe it is a date? search a record by date
		if (auto didx = KDate::fromString(sidx); didx.valid())
		{
			for (auto& d : m_work)
			{
				if (d.read().date.str() == didx.str())
				{
					d.edit();
					return;
				}
			}
		}
		std::cout << "Unable to find registered index or date\n";
	}


	std::string print_short() const
	{
		std::ostringstream ss;
		switch (m_work.size())
		{
		case 0:  return "<Work registry empty>";
		case 1:  ss << "1 record: " << m_work.begin()->print_full(); break;
		case 2:  ss << "2 records: " << m_work.begin()->datestr() << ", " << m_work.rbegin()->datestr(); break;
		default: ss << m_work.size() << " records, " << m_work.begin()->datestr() << " .. " << m_work.rbegin()->datestr(); break;
		}
		if (!valid())
			ss << " <Invalid>";
		return ss.str();
	}

	std::string print_full() const
	{
		if (empty())
		{
			return "<Work registry empty>";
		}

		std::ostringstream ss;
		for (size_t n = 0; n < m_work.size(); ++n)
		{
			const auto& w = m_work.at(n);
			ss << (n + 1) << "\t" << w.print_full() << "\n";
		}
		return ss.str();
	}

	void add_edit()
	{
		KWorkday w;
		w.input_record();
		if (w.valid())
		{
			add(w);
		}
		else
		{
			std::cout << "Failed to add, returning to menu\n";
		}
	}

	void save() const
	{
		try
		{
			{
				std::ofstream fs(database_filename, std::ios::binary);
				fs << serialize();
			}
			std::cout << " Saved " << std::filesystem::file_size(database_filename) << " bytes into " << database_filename << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << " Failed to save " << database_filename << ": " << e.what() << std::endl;
		}
	}

	std::string serialize() const
	{
		std::ostringstream ss;
		for (const auto& w : m_work)
			ss << w.read().date.str() << " " << w.read().start << " " << w.read().end << "\n";
		return ss.str();
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
			deserialize(buffer.str());
		}
		catch (const std::exception& e)
		{
			std::cerr << " Failed to load " << database_filename << ": " << e.what() << std::endl;
		}
	}

	void deserialize(std::string s, bool doprint = true)
	{
		clear();
		std::stringstream is(s);
		std::string line;
		while (std::getline(is, line))
		{
			if (line.empty() || line[0] == '#')
				continue; // commented or empty lines

			if (KWorkday d(line); d.valid())
			{
				add(d);
				if(doprint)
					std::cout << "(+) " << line << "\n";
			}
			else
			{
				if (doprint)
					std::cout << "(-) " << line << "\n";
			}
		}
	}

};