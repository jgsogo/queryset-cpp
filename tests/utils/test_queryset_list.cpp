
#include <boost/test/unit_test.hpp>

#include "print_helper.hpp"
#include "../../queryset/utils/queryset.h"

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


BOOST_FIXTURE_TEST_SUITE(queryset_list, Fixture)

BOOST_AUTO_TEST_CASE(basic)
{
    auto int_list = ::utils::list<int>(qs);
    BOOST_CHECK_EQUAL(int_list.size(), 3);
    BOOST_CHECK_EQUAL(int_list.at(0), 0);
    BOOST_CHECK_EQUAL(int_list[1], 1);
    BOOST_CHECK_EQUAL(*int_list.rbegin(), 2);

    auto str_list = ::utils::list<std::string>(qs);
    BOOST_CHECK_EQUAL(str_list.size(), 3);
    BOOST_CHECK_EQUAL(str_list.at(0), "hola");
    BOOST_CHECK_EQUAL(str_list[1], "bye");
    BOOST_CHECK_EQUAL(*str_list.rbegin(), "ciao");

    auto float_list = ::utils::list<float>(qs);
    BOOST_CHECK_EQUAL(float_list.size(), 3);
    BOOST_CHECK_EQUAL(float_list.at(0), 3.f);
    BOOST_CHECK_EQUAL(float_list[1], 2.f);
    BOOST_CHECK_EQUAL(*float_list.rbegin(), 1.f);
}

BOOST_AUTO_TEST_SUITE_END()
