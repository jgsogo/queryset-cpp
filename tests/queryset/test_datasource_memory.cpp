
#include <boost/test/unit_test.hpp>

#include "../print_helper.hpp"
#include "../../queryset/datasource.h"

typedef std::tuple<int, std::string, float> MyModelTuple;
typedef qs::MemoryDataSource<MyModelTuple, int, std::string, float> myDataSource;



BOOST_AUTO_TEST_SUITE(datasource_memory)

BOOST_AUTO_TEST_CASE(basic)
{
    myDataSource memqs;
    BOOST_CHECK_EQUAL(memqs.size(), 0);

    // Append some tuples
    memqs >> MyModelTuple{1, "hola", 1.f};
    memqs >> MyModelTuple{2, "bye", 2.f};
    memqs >> MyModelTuple{3, "ciao", 3.f};
    BOOST_CHECK_EQUAL(memqs.size(), 3);
    memqs >> MyModelTuple{3, "ciao", 3.f};
    BOOST_CHECK_EQUAL(memqs.size(), 4);
}

BOOST_AUTO_TEST_SUITE_END()
