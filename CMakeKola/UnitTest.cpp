// Primitive unit test
// TODO: use true test framework such as Catch2 or GTest

#include "KHour.h"
#include "KDate.h"
#include "KWorkRegistry.h"
#include "KParameters.h"

Ledger calculate(const KParameters& parameters, const KWorkRegistry& workdays);

void Test_KHour()
{
	KHour empty;
	assert(!empty.valid());
	assert(empty.str() == "N/A");

	KHour invalid = KHour(200);
	assert(!invalid.valid());
	assert(invalid.str() == "N/A");

	KHour invalstr = KHour::fromString("200");
	assert(!invalstr.valid());
	assert(invalstr.str() == "N/A");

	KHour goodstr = KHour::fromString("13.5");
	assert(goodstr.valid());
	assert(goodstr.str() == "13:30");
}

void Test_KDate()
{
	KDate empty;
	assert(!empty.valid());
	assert(empty.str() == "");

	KDate invalid = KDate::fromString("200");
	assert(!invalid.valid());
	assert(invalid.str() == "");

	KDate goodstr = KDate::fromString("2024-01-02");
	assert(goodstr.valid());
	assert(goodstr.str() == "2024-01-02");
}

void Test_KWorkday()
{
	KWorkday empty;
	assert(!empty.valid());
	assert(empty.datestr() == "");

	KWorkday apr1(KDate::fromString("2024-04-01"), KHour(7.5), KHour(17.5));
	assert(apr1.valid());
	auto s1 = apr1.print_full();
	assert(s1 == "2024-04-01 07:30..17:30");

	KWorkday apr1input("2024-04-01 7.5 17.5");
	assert(apr1input.valid());
	auto s2 = apr1input.print_full();
	assert(s2 == "2024-04-01 07:30..17:30");

	assert(s1 == s2);
	assert(!(apr1 < apr1input));
	assert(!(apr1input < apr1));

	KWorkday dec30input("2024-12-30 0.5 1.5");
	assert(dec30input.valid());
	assert(apr1input < dec30input);

	KWorkday dec30input2("2024-12-30 1.5 2.0");
	assert(dec30input2.valid());
	assert(dec30input < dec30input2);

	KWorkday longhours("2024-12-30 1.5 22.");
	assert(longhours.valid());  // todo epsilon comparison
	assert(5.5 == longhours.working_hours_night());  // 01:30-07.00
	assert(10. == longhours.working_hours_day());    // 07.00-17.00
	assert(5. == longhours.working_hours_evening()); // 17.00-22.00

	KWorkday evhours("2024-12-30 19. 22.");
	assert(evhours.valid());  // todo epsilon comparison
	assert(0 == evhours.working_hours_night());
	assert(0. == evhours.working_hours_day());
	assert(3. == evhours.working_hours_evening());
}

void Test_KWorkRegistry()
{
	KWorkRegistry kw0;
	assert(kw0.empty());
	assert(kw0.serialize() == "");

	KWorkRegistry kw1;
	kw1.add(KWorkday("2024-04-01 7.5 17.5"));
	assert(!kw1.empty());
	assert(kw1.valid());
	assert(kw1.serialize() == "2024-04-01 7.5 17.5\n");

	KWorkRegistry kw2 = kw1;
	kw2.add(KWorkday("2024-04-02 8.5 9.5"));
	assert(!kw2.empty());
	assert(kw2.valid());
	assert(kw2.serialize() == "2024-04-01 7.5 17.5\n2024-04-02 8.5 9.5\n");

	KWorkRegistry kw22;
	kw22.deserialize(kw2.serialize(), false);
	assert(!kw22.empty());
	assert(kw22.valid());
	assert(kw22.serialize() == kw2.serialize());
}

void Test_KParameters()
{
	KParameters empty;
	assert(!empty.valid());
	assert(empty.print_short() == "<Parameters not set>");

	KParameters def;
	def.set_example();
	auto s = def.print_short();
	assert(def.print_short() == "$145.5/h Wknd/Evng: 23.5/15.5 Tax: 25%");
}

void Test_Ledger()
{
	KParameters def;
	def.set_example();

	KWorkRegistry kw2;
	kw2.deserialize("2024-04-01 7.5 17.5\n2024-04-02 8.5 9.5\n", false);

	auto res = calculate(def, kw2);

	assert(res.tax == 25);
	assert(res.ledger.size() == 2);
	assert(res.total == 1608.25);
	assert(res.total_after_tax == 1206.1875);
}

// run all tests
void Test()
{
	Test_KHour();
	Test_KDate();
	Test_KWorkday();
	Test_KWorkRegistry();
	Test_KParameters();
	Test_Ledger();
}