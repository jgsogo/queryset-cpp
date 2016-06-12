
#include <boost/test/unit_test.hpp>

#include "../utils/print_helper.hpp"
#include "../../queryset/datasource.h"

typedef ::utils::MemoryQueryset<int, std::string, float> myQuerySet;
typedef myQuerySet::qs_type::value_type mytuple;


BOOST_AUTO_TEST_SUITE(datasource_memory)

BOOST_AUTO_TEST_CASE(basic)
{
    myQuerySet memqs;
    BOOST_CHECK_EQUAL(memqs.size(), 0);

    // Append some tuples
    memqs >> mytuple{1, "hola", 1.f};
    memqs >> mytuple{2, "bye", 2.f};
    memqs >> mytuple{3, "ciao", 3.f};
    BOOST_CHECK_EQUAL(memqs.size(), 3);
    memqs >> mytuple{3, "ciao", 3.f};
    BOOST_CHECK_EQUAL(memqs.size(), 4);
}

BOOST_AUTO_TEST_SUITE_END()
