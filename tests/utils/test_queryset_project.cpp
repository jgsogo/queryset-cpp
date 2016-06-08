
#include <boost/test/unit_test.hpp>

#include "print_helper.hpp"
#include "../../src/utils/queryset.h"

typedef ::utils::queryset<int, std::string, float> myqueryset;
typedef std::tuple<int, std::string, float> mytuple;

struct Fixture {
    Fixture() {
        qs.push_back(mytuple{ 0, "hola", 3.f });
        qs.push_back(mytuple{ 1, "bye", 2.f });
        qs.push_back(mytuple{ 2, "ciao", 1.f });

        ps = qs;
    }

    ~Fixture() { BOOST_TEST_MESSAGE("teardown fixture"); }

    myqueryset qs;
    myqueryset ps;
};


BOOST_FIXTURE_TEST_SUITE(queryset_project, Fixture)

BOOST_AUTO_TEST_CASE(compound)
{
    auto projected_qs = ::utils::project<int, std::string>(qs);
    BOOST_CHECK_EQUAL(projected_qs.size(), 3);
    BOOST_CHECK_EQUAL(projected_qs.at(0), (std::tuple<int, std::string>{0, "hola"}));
    BOOST_CHECK_EQUAL(projected_qs[1], (std::tuple<int, std::string>{1, "bye"}));
    BOOST_CHECK_EQUAL(*projected_qs.rbegin(), (std::tuple<int, std::string>{2, "ciao"}));
}

BOOST_AUTO_TEST_SUITE_END()
