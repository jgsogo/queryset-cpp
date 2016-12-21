
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>  // for execution_exception

#include "../print_helper.hpp"
#include "../../queryset/queryset.h"


typedef ::qs::MemoryDataSource<std::size_t, std::string, float> myDataSource;
typedef std::tuple<std::size_t, std::string, float> mytuple;

struct FixtureGet {
    FixtureGet() {
        initial_qs >> mytuple{ 0, "hola", 0.f };
        initial_qs >> mytuple{ 1, "bye", 0.1f };
        initial_qs >> mytuple{ 2, "ciao", 0.2f };

        initial_qs >> mytuple{ 3, "hola", 1.f };
        initial_qs >> mytuple{ 4, "bye", 1.1f };
        initial_qs >> mytuple{ 5, "ciao", 1.2f };

        initial_qs >> mytuple{ 6, "hola", 2.f };
        initial_qs >> mytuple{ 7, "bye", 2.1f };
        initial_qs >> mytuple{ 7, "ciao", 2.2f };
    }
    ~FixtureGet() {}

    myDataSource initial_qs;
};

BOOST_FIXTURE_TEST_SUITE(queryset_class, FixtureGet)

BOOST_AUTO_TEST_CASE(operator_get)
{
    QuerySet<std::size_t, std::string, float> qs(initial_qs);
    auto t0 = qs.get(0);
    BOOST_CHECK_EQUAL(std::get<0>(t0), 0);
    BOOST_CHECK_EQUAL(std::get<1>(t0), "hola");
    BOOST_CHECK_EQUAL(std::get<2>(t0), 0.f);

    auto t4 = qs.get(4);
    BOOST_CHECK_EQUAL(std::get<0>(t4), 4);
    BOOST_CHECK_EQUAL(std::get<1>(t4), "bye");
    BOOST_CHECK_EQUAL(std::get<2>(t4), 1.1f);
}

BOOST_AUTO_TEST_CASE(operator_get_notfound)
{
    QuerySet<std::size_t, std::string, float> qs(initial_qs);

    typedef qs::not_found<std::size_t> not_found_exception;
    BOOST_REQUIRE_THROW(qs.get(10), not_found_exception);
}

BOOST_AUTO_TEST_CASE(operator_get_multiple)
{
    QuerySet<std::size_t, std::string, float> qs(initial_qs);

    typedef qs::multiple_found<std::size_t> mutiple_found_exception;
    BOOST_REQUIRE_THROW(qs.get(7), qs::multiple_found<std::size_t>);
}


BOOST_AUTO_TEST_SUITE_END()

