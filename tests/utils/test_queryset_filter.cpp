
#include <boost/test/unit_test.hpp>

#include "../print_helper.hpp"
#include "../../queryset/utils/queryset.h"

typedef ::utils::queryset<int, std::string, float> myqueryset;
typedef std::tuple<int, std::string, float> mytuple;

BOOST_AUTO_TEST_SUITE(queryset_filter)

BOOST_AUTO_TEST_CASE(filter_single)
{
    myqueryset qs;
    qs.push_back(mytuple{ 0, "hola", 3.f });
    qs.push_back(mytuple{ 1, "bye", 2.f });
    qs.push_back(mytuple{ 2, "ciao", 1.f });

    auto filtered_int = ::utils::filter(qs, 0);
    BOOST_CHECK_EQUAL(filtered_int.size(), 1);
    BOOST_CHECK_EQUAL(filtered_int.at(0), (mytuple{ 0, "hola", 3.f }));

    auto filtered_float = ::utils::filter(qs, 2.f);
    BOOST_CHECK_EQUAL(filtered_float.size(), 1);
    BOOST_CHECK_EQUAL(filtered_float.at(0), (mytuple{ 1, "bye", 2.f }));

}

BOOST_AUTO_TEST_CASE(filter_vector)
{
    myqueryset qs;
    qs.push_back(mytuple{ 0, "hola", 3.f });
    qs.push_back(mytuple{ 1, "bye", 2.f });
    qs.push_back(mytuple{ 2, "ciao", 1.f });

    auto filtered_int = ::utils::filter(qs, std::vector<int>{0, 1});
    BOOST_CHECK_EQUAL(filtered_int.size(), 2);
    BOOST_CHECK_EQUAL(filtered_int.at(0), (mytuple{ 0, "hola", 3.f }));
    BOOST_CHECK_EQUAL(filtered_int.at(1), (mytuple{ 1, "bye", 2.f }));

}

BOOST_AUTO_TEST_CASE(filter_tuple)
{
    myqueryset qs;
    qs.push_back(mytuple{ 0, "hola", 3.f });
    qs.push_back(mytuple{ 1, "bye", 2.f });
    qs.push_back(mytuple{ 2, "ciao", 1.f });

    auto filtered_int = ::utils::filter(qs, std::tuple<int, std::string>{0, "hola"});
    BOOST_CHECK_EQUAL(filtered_int.size(), 1);
    BOOST_CHECK_EQUAL(filtered_int.at(0), (mytuple{ 0, "hola", 3.f }));

    auto filtered_tw = ::utils::filter(qs, std::tuple<int, std::string>{0, "bye"});
    BOOST_CHECK_EQUAL(filtered_tw.size(), 0);
}


BOOST_AUTO_TEST_SUITE_END()
