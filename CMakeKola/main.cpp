#include <iostream>

#include "KParameters.h"
#include "KWorkRegistry.h"
#include "KConsole.h"
#include <iomanip> // for std::setw and std::setfill TODO: use std::print in new C++

struct MainData
{
	KParameters parameters;
	KWorkRegistry workdays;
};

std::string composeMainInfo(const MainData& main)
{
	return "Main | " + main.parameters.print_short() + " | " + main.workdays.print_short();
}

Choices composeMainMenu(const MainData& main)
{
	Choices mainMenu = { {'X', "Exit"}, {'P', "Parameters"}, {'D', "Working days"} };

	if (main.parameters.valid() && main.workdays.valid())
		mainMenu.emplace_back('C', "Calculate $");

	return mainMenu;
}

// Note: don't forget to also add test calculations into UnitTest.cpp
Ledger calculate(const KParameters& parameters, const KWorkRegistry& workdays)
{
	assert(workdays.valid());
	assert(parameters.valid());
	Ledger res;
	res.ledger.reserve(workdays.read().size());
	res.tax = parameters.read().tax;

	for (const auto& w : workdays.read())
	{
		const double day_wage = parameters.read().hourly_wage
			+ (w.read().date.weekend() ? parameters.read().supplement_in_weekend : 0.);

		const double night_wage = day_wage + parameters.read().supplement_in_evening;

		const auto day_hours = w.working_hours_day();

		const auto night_hours = w.working_hours_evening() + w.working_hours_night();

		Ledge ledge;
		ledge.day = day_hours * day_wage;
		ledge.night = night_hours * night_wage;
		ledge.total = ledge.day + ledge.night;
		ledge.pWorkday = &w;
		res.total += ledge.total;
		res.ledger.emplace_back(ledge);
	}
	res.total_after_tax = res.total * ((100. - res.tax) / 100.);
	return res;
}

void printLedger(const Ledger& report)
{
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(2) << std::showpoint;

	ss << std::setw(12) << "Total"
		<< std::setw(12) << "Day"
		<< std::setw(12) << "Night"
		<< std::setw(30) << "Workday\n";

	if (report.ledger.empty())
		ss << "<Empty>\n";

	for (const auto& l : report.ledger)
	{
		ss << std::setw(12) << l.total
			<< std::setw(12) << l.day
			<< std::setw(12) << l.night
			<< std::setw(30) << l.pWorkday->print_full()
			<< "\n";
	}
	ss << std::setw(12) << "__________" << "\n";
	ss << std::setw(12) << report.total << "\n";
	ss << std::setw(12) << report.total_after_tax << " after tax deduction (" << report.tax << "%)";

	std::cout << ss.str() << std::endl;
}

// Unit Test
void Test();

void mainLoop()
{
#ifdef _DEBUG
	// run self-diagnostic
	Test();
#endif // _DEBUG

	MainData main;
	while (true)
	{
		char choice = UserChoice(composeMainInfo(main), composeMainMenu(main));
		switch (choice)
		{
			case 'X': std::cout << "Finishing!\n"; return;
			case 'P': main.parameters.edit(); break;
			case 'D': main.workdays.edit(); break;
			case 'C': printLedger(calculate(main.parameters, main.workdays)); break;
			default: throw std::invalid_argument("Invalid menu item");
		}
	}
}

int main()
{
	std::cout << "Welcome to KolaKola!\n";
	try
	{
		mainLoop();
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << std::endl;
		std::cerr << "Main terminated. Reason: " << e.what() << "\n";
		return 1;
	}
	catch (...)
	{
		std::cout << std::endl;
		std::cerr << "Main terminated. Unknown exception." << "\n";
		return 1;
	}
}
