
#include <boost/test/unit_test.hpp>

#include "../print_helper.hpp"
#include "../../queryset/datasource.h"

typedef qs::MemoryDataSource<int, std::string, float> myDataSource;
typedef myDataSource::qs_type::value_type mytuple;


BOOST_AUTO_TEST_SUITE(datasource_memory)

BOOST_AUTO_TEST_CASE(basic)
{
    myDataSource memqs;
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
