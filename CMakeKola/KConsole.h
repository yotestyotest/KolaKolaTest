#pragma once

// Wraps interactive console - reading of values and menu navigation

#include <string>
#include <string_view>
#include <iostream>
#include <map>
#include <cassert>
#include <sstream>
#include <conio.h> // For _getch()

using Choices = std::vector<std::pair<char, std::string>>;

constexpr char KEY_ESC = 27;
constexpr char KEY_CTRLC = 3;

inline std::map<char, std::string> no_duplicates(const Choices& menu)
{
	std::map<char, std::string> res(menu.cbegin(), menu.cend());
	if (res.size() != menu.size())
		throw std::invalid_argument("Invalid menu: same shortcut used twice");
	return res;
}

inline char UserChoice(std::string_view info, Choices vmenu)
{
	auto menu = no_duplicates(vmenu);
	assert(!menu.empty() && "Wrong menu");

	std::cout << std::endl;

	if (!info.empty())
		std::cout << info << "\n";

	for (const auto& [choice_c, choice_name] : vmenu)
		std::cout << "[" << choice_c << "] " << choice_name << " ";

	while(true)
	{
		char choice = _getch();  // _getch() allows to wait for just one char on Windows; there is another options on Linux if necessary
		if (choice == KEY_CTRLC || choice == KEY_ESC)
			throw std::runtime_error("Terminated from user input");
		if (choice < '!')
			continue; // nonprint

		std::cout << choice << '\b';
		choice = std::toupper(choice); // x --> X
		if (auto menuitr = menu.find(choice); menuitr != menu.end())
		{
			std::cout << "\n > " << menuitr->second << "\n";
			return choice;
		}
	};
}

// read double from keyboard
inline double DoubleInput(std::string_view prompt, double defaultValue, double allowMin = std::numeric_limits<double>::min(), double allowMax = std::numeric_limits<double>::max())
{
	while (true)
	{
		if (!prompt.empty())
			std::cout << prompt << " [" << defaultValue << "]: ";
		std::string inpline;
		std::getline(std::cin, inpline);
		if (inpline.empty())
			return defaultValue; // user just pressed Enter

		double val{};
		if ((std::stringstream(inpline) >> val) && allowMin <= val && val <= allowMax)
			return val;

		std::cout << " Please input a value between " << allowMin << " and " << allowMax << ", inclusively.\n";
		std::cout << " Press enter if you agree with " << prompt << " = " << defaultValue << "\n";
	}
}

// change double value in place from keyboard
inline void DoubleInput(double& value, std::string_view prompt, double allowMin = std::numeric_limits<double>::min(), double allowMax = std::numeric_limits<double>::max())
{
	value = DoubleInput(prompt, value, allowMin, allowMax);
}


// read string from keyboard
inline std::string StringInput(std::string_view prompt, std::string_view defaultValue, int minChars = 0, int maxChars = 1000)
{
	while (true)
	{
		if (!prompt.empty())
			std::cout << prompt << " [" << defaultValue << "]: ";
		std::string inpline;
		std::getline(std::cin, inpline);
		if (inpline.empty())
			return std::string(defaultValue); // user just pressed Enter

		const auto len = inpline.size();
		if (minChars <= len && len <= maxChars)
			return inpline;

		std::cout << " Please input a text of length between " << minChars << " and " << maxChars << ", inclusively.\n";
		std::cout << " Press enter if you agree with " << prompt << " = " << defaultValue << "\n";
	}
}
