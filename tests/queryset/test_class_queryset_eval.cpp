
#include <boost/test/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>  // for execution_exception

#include "../print_helper.hpp"
#include "../../queryset/queryset.h"

typedef std::tuple<int, std::string, float> MyModelTuple;
typedef qs::MemoryDataSource<MyModelTuple, int, std::string, float> myDataSource;
//typedef std::tuple<int, std::string, float> mytuple;

struct Fixture {
    Fixture() {
        initial_qs >> MyModelTuple{ 0, "hola", 0.f };
        initial_qs >> MyModelTuple{ 0, "bye", 0.1f };
        initial_qs >> MyModelTuple{ 0, "ciao", 0.2f };

        initial_qs >> MyModelTuple{ 1, "hola", 1.f };
        initial_qs >> MyModelTuple{ 1, "bye", 1.1f };
        initial_qs >> MyModelTuple{ 1, "ciao", 1.2f };

        initial_qs >> MyModelTuple{ 2, "hola", 2.f };
        initial_qs >> MyModelTuple{ 2, "bye", 2.1f };
        initial_qs >> MyModelTuple{ 2, "ciao", 2.2f };
    }
    ~Fixture() {}

    myDataSource initial_qs;
};

BOOST_FIXTURE_TEST_SUITE(queryset_class, Fixture)

BOOST_AUTO_TEST_CASE(eval_once)
{
    QuerySet<MyModelTuple, int, std::string, float> qs(initial_qs);
    auto f0 = qs.filter(0);
    BOOST_CHECK_EQUAL(f0.count(), 3);
    #ifdef _DEBUG
    BOOST_REQUIRE_THROW(f0.filter(1), boost::execution_exception);
    #endif
    
    auto qs2 = qs; // qs2 copy from qs (already filtered for int=0)
    auto f1 = qs2.filter(1);
    BOOST_CHECK_EQUAL(f1.count(), 0);
    #ifdef _DEBUG
    BOOST_REQUIRE_THROW(f1.filter(0), boost::execution_exception);
    #endif
}

BOOST_AUTO_TEST_SUITE_END()

