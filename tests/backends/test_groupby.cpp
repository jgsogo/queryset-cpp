
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>

#include "../print_helper.hpp"
#include "../../queryset/queryset.h"
#include "fixture_datasource.hpp"

typedef boost::mpl::list<FixtureDataSource<MemoryDataSource>,
                         FixtureDataSource<FileDataSource>,
                         FixtureDataSource<SQLite3DataSource>> test_types;


BOOST_AUTO_TEST_SUITE(common_groupby)

BOOST_FIXTURE_TEST_CASE_TEMPLATE(groupby_single, T, test_types, T)
{
    // TODO: La ejecuci�n del group_by se puede delegar hasta la base de datos (en teor�a ser� m�s r�pida que la que se hace en C++)
    QuerySet<int, std::string, float> qs(T::datasource);
    auto f4 = qs.groupBy<std::string>();
    BOOST_CHECK_EQUAL(f4.count(), 3);

    BOOST_CHECK_EQUAL(f4.at("jgsogo").count(), 2);
    BOOST_CHECK_EQUAL(f4.at("conan").count(), 1);
    BOOST_CHECK_EQUAL(f4.at("lasote").count(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
