
#include <boost/test/unit_test.hpp>

#include "../print_helper.hpp"
#include "../../queryset/utils/to_string.h"

struct NotToStringMethod {
	std::string value;
};
//std::ostream& operator << (std::ostream& os, const NotToStringMethod& item) { os << item.value; return os; }

struct StringMethod {
	std::string value;
	std::string to_string() const { return "method/to_string"; }
};

BOOST_AUTO_TEST_SUITE(to_string)

BOOST_AUTO_TEST_CASE(simple_types)
{
	BOOST_CHECK_EQUAL((::utils::to_string(std::string("std::string"))), "std::string");
	BOOST_CHECK_EQUAL((::utils::to_string(3)), "3");
}

BOOST_AUTO_TEST_CASE(not_to_string_method)
{
	NotToStringMethod item; item.value = "NotToStringMethod";
	//BOOST_CHECK_EQUAL((::utils::to_string(item)), "NotToStringMethod");
}

BOOST_AUTO_TEST_CASE(to_string_method)
{
	std::string value = "StringMethod::value";
	StringMethod item; item.value = value;
	BOOST_CHECK((::utils::to_string(item)) != value);
	BOOST_CHECK_EQUAL((::utils::to_string(item)), item.to_string());
}




BOOST_AUTO_TEST_SUITE_END()
