
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../print_helper.hpp"
#include "../../queryset/queryset.h"
#include "fixture_datasource.hpp"


BOOST_FIXTURE_TEST_SUITE(backends_sqlite3_save, FixtureDataSource<SQLite3DataSource>)

BOOST_AUTO_TEST_CASE(add_one)
{
    QuerySet<MyModelTuple, int, std::string, float> qs(datasource);
    BOOST_CHECK_EQUAL(qs.count(), 4);
    
    // Add one item to db
    std::tuple<int, std::string, float> item{ 5, "sqlite3", 1.0 };
    datasource.insert(item);
    BOOST_CHECK_EQUAL(qs.count(), 4); // Query cache needs to be reseted.
    qs.reset();
    BOOST_CHECK_EQUAL(qs.count(), 5);
}

BOOST_AUTO_TEST_CASE(delete_one)
{
    {
        QuerySet<MyModelTuple, int, std::string, float> qs(datasource);
        BOOST_CHECK_EQUAL(qs.count(), 4);
    }
    {
        QuerySet<MyModelTuple, int, std::string, float> qs(datasource);
        qs.filter(1);
        BOOST_CHECK_EQUAL(qs.count(), 1);
        qs.remove();
    }
    {
        QuerySet<MyModelTuple, int, std::string, float> qs(datasource);
        BOOST_CHECK_EQUAL(qs.count(), 3);
    }
}

BOOST_AUTO_TEST_SUITE_END()