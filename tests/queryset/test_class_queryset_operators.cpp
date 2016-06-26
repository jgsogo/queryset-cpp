
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>  // for execution_exception

#include "../print_helper.hpp"
#include "../../queryset/queryset.h"


typedef qs::MemoryQueryset<int, std::string, float> myqueryset;
typedef std::tuple<int, std::string, float> mytuple;

struct Fixture {
    Fixture() {
        initial_qs >> mytuple{ 0, "hola", 0.f };
        initial_qs >> mytuple{ 0, "bye", 0.1f };
        initial_qs >> mytuple{ 0, "ciao", 0.2f };

        initial_qs >> mytuple{ 1, "hola", 1.f };
        initial_qs >> mytuple{ 1, "bye", 1.1f };
        initial_qs >> mytuple{ 1, "ciao", 1.2f };

        initial_qs >> mytuple{ 2, "hola", 2.f };
        initial_qs >> mytuple{ 2, "bye", 2.1f };
        initial_qs >> mytuple{ 2, "ciao", 2.2f };
    }
    ~Fixture() {}

    myqueryset initial_qs;
};

BOOST_FIXTURE_TEST_SUITE(queryset_class, Fixture)

BOOST_AUTO_TEST_CASE(operator_random_access)
{
    QuerySet<int, std::string, float> qs(initial_qs);
    auto f0 = qs[0];
    BOOST_CHECK_EQUAL(std::get<0>(f0), 0);
    BOOST_CHECK_EQUAL(std::get<1>(f0), "hola");
    BOOST_CHECK_EQUAL(std::get<2>(f0), 0.f);

    auto f4 = qs[4];
    BOOST_CHECK_EQUAL(std::get<0>(f4), 1);
    BOOST_CHECK_EQUAL(std::get<1>(f4), "bye");
    BOOST_CHECK_EQUAL(std::get<2>(f4), 1.1f);
}

BOOST_AUTO_TEST_CASE(iterator)
{
    QuerySet<int, std::string, float> qs(initial_qs);
    auto b = qs.begin();
    auto e = qs.end();
    BOOST_CHECK_EQUAL(std::get<0>(*b), 0);
    BOOST_CHECK_EQUAL(std::get<1>(*b), "hola");
    BOOST_CHECK_EQUAL(std::get<2>(*b), 0.f);
    BOOST_CHECK_EQUAL(std::distance(b, e), 9);

    auto rb = qs.rbegin();
    auto re = qs.rend();
    BOOST_CHECK_EQUAL(std::distance(rb, re), 9);
    
    BOOST_CHECK_EQUAL(*rb, *(--e));
    BOOST_CHECK_EQUAL(*b, *(--re));

    // for-loop
    for (auto item : qs) {}
}

BOOST_AUTO_TEST_SUITE_END()

