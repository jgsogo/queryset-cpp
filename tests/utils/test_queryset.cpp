
#include <boost/test/unit_test.hpp>

#include "../print_helper.hpp"
#include "../../queryset/utils/queryset.h"

typedef ::utils::queryset<int, std::string, float> myqueryset;
typedef std::tuple<int, std::string, float> mytuple;

struct Fixture {
    Fixture() {
        qs.push_back(mytuple{ 0, "hola", 3.f });
        qs.push_back(mytuple{ 1, "bye", 2.f });
        qs.push_back(mytuple{ 2, "ciao", 1.f });
    }
    
    ~Fixture() { BOOST_TEST_MESSAGE("teardown fixture"); }

    myqueryset qs;
};


BOOST_FIXTURE_TEST_SUITE(queryset, Fixture)

BOOST_AUTO_TEST_CASE(basic_tests)
{
    BOOST_CHECK_EQUAL(qs.size(), 3);
    BOOST_CHECK_EQUAL(qs.at(0), (mytuple{ 0, "hola", 3.f }));
    BOOST_CHECK_EQUAL(qs[1], (mytuple{ 1, "bye", 2.f }));
    BOOST_CHECK_EQUAL(*qs.rbegin(), (mytuple{ 2, "ciao", 1.f }));
}

BOOST_AUTO_TEST_CASE(marginalization)
{
    auto marginalized_int = ::utils::marginalize<int>(qs);
    auto marginalized_str = ::utils::marginalize<std::string>(qs);
    auto marginalized_float = ::utils::marginalize<float>(qs);

    std::size_t i = 0;
    for (myqueryset::const_iterator it = qs.begin(); it != qs.end(); ++it, ++i) {
        BOOST_CHECK_EQUAL(marginalized_int.at(i), (::utils::tuple::project<1, 2>(*it)));
        BOOST_CHECK_EQUAL(marginalized_str.at(i), (::utils::tuple::project<0, 2>(*it)));
        BOOST_CHECK_EQUAL(marginalized_float.at(i), (::utils::tuple::project<0, 1>(*it)));
    }
}


BOOST_AUTO_TEST_SUITE_END()
