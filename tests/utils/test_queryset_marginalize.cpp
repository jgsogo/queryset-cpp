
#include <boost/test/unit_test.hpp>

#include "../print_helper.hpp"
#include "../../queryset/utils/queryset.h"

typedef ::utils::queryset<int, std::string, float> myqueryset;
typedef std::tuple<int, std::string, float> mytuple;

BOOST_AUTO_TEST_SUITE(queryset_marginalize)

BOOST_AUTO_TEST_CASE(basic)
{
    myqueryset qs;
    qs.push_back(mytuple{ 0, "hola", 3.f });
    qs.push_back(mytuple{ 1, "bye", 2.f });
    qs.push_back(mytuple{ 2, "ciao", 1.f });

    auto marginalized_int = ::utils::marginalize<int>(qs);
    BOOST_CHECK_EQUAL(marginalized_int.size(), 3);
    BOOST_CHECK_EQUAL(marginalized_int.at(0), (std::tuple<std::string, float>{ "hola", 3.f }));
    BOOST_CHECK_EQUAL(marginalized_int.at(1), (std::tuple<std::string, float>{ "bye", 2.f }));
    BOOST_CHECK_EQUAL(marginalized_int.at(2), (std::tuple<std::string, float>{ "ciao", 1.f }));

    auto marginalized_str = ::utils::marginalize<std::string>(qs);
    BOOST_CHECK_EQUAL(marginalized_str.size(), 3);
    BOOST_CHECK_EQUAL(marginalized_str.at(0), (std::tuple<int, float>{ 0, 3.f }));
    BOOST_CHECK_EQUAL(marginalized_str.at(1), (std::tuple<int, float>{ 1, 2.f }));
    BOOST_CHECK_EQUAL(marginalized_str.at(2), (std::tuple<int, float>{ 2, 1.f }));

    auto marginalized_float = ::utils::marginalize<float>(qs);
    BOOST_CHECK_EQUAL(marginalized_float.size(), 3);
    BOOST_CHECK_EQUAL(marginalized_float.at(0), (std::tuple<int, std::string>{ 0, "hola" }));
    BOOST_CHECK_EQUAL(marginalized_float.at(1), (std::tuple<int, std::string>{ 1, "bye" }));
    BOOST_CHECK_EQUAL(marginalized_float.at(2), (std::tuple<int, std::string>{ 2, "ciao" }));
}

BOOST_AUTO_TEST_SUITE_END()
