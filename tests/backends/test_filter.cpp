
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>

#include "../print_helper.hpp"
#include "fixture_datasource.hpp"

typedef boost::mpl::list<FixtureDataSource<MemoryDataSource>,
                         FixtureDataSource<FileDataSource>,
                         FixtureDataSource<SQLite3DataSource>> test_types;

BOOST_AUTO_TEST_SUITE(common_filter)

BOOST_FIXTURE_TEST_CASE_TEMPLATE(basic, T, test_types, T)
{
    qs::FilterContainer<MyModelTuple, int, std::string, float> filters;
    auto qs = T::datasource.apply(filters);

    BOOST_CHECK(qs.size() == 4);
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE(filter_one_string, T, test_types, T)
{
    qs::FilterContainer<MyModelTuple, int, std::string, float> filters;
	filters.add_filter<std::string>("jgsogo");
	auto qs = T::datasource.apply(filters);

	BOOST_CHECK_EQUAL(qs.size(), 2);
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE(filter_one_numeric, T, test_types, T)
{
	qs::FilterContainer<MyModelTuple, int, std::string, float> filters;
	filters.add_filter<float>(1.0);
	auto qs = T::datasource.apply(filters);

	BOOST_CHECK_EQUAL(qs.size(), 2);
}

/*
BOOST_FIXTURE_TEST_CASE_TEMPLATE(filter_several_string, T, test_types, T, *boost::unit_test::disabled())
{
    // TODO: To enable this test I need REGEXP function on sqlite3.
	qs::FilterContainer<int, std::string, float> filters;
	filters.add_filter<std::string>({ "jgsogo", "conan" });
	auto qs = T::datasource.apply(filters);

	BOOST_CHECK_EQUAL(qs.size(), 3);
}
*/

BOOST_FIXTURE_TEST_CASE_TEMPLATE(filter_several_numeric, T, test_types, T)
{
	qs::FilterContainer<MyModelTuple, int, std::string, float> filters;
	filters.add_filter<float>({ 1.0f, 0.9f });
	auto qs = T::datasource.apply(filters);

	BOOST_CHECK_EQUAL(qs.size(), 4);
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE(filter_combined, T, test_types, T)
{
	qs::FilterContainer<MyModelTuple, int, std::string, float> filters;
	filters.add_filter<std::string>("jgsogo");
	filters.add_filter<float>(1.0);
	auto qs = T::datasource.apply(filters);

	BOOST_CHECK_EQUAL(qs.size(), 1);
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE(filter_no_match_string, T, test_types, T)
{
	qs::FilterContainer<MyModelTuple, int, std::string, float> filters;
	filters.add_filter<std::string>("----");
	auto qs = T::datasource.apply(filters);

	BOOST_CHECK_EQUAL(qs.size(), 0);
}

BOOST_FIXTURE_TEST_CASE_TEMPLATE(filter_no_match_numeric, T, test_types, T)
{
    qs::FilterContainer<MyModelTuple, int, std::string, float> filters;
    filters.add_filter<int>(23);
    auto qs = T::datasource.apply(filters);

    BOOST_CHECK_EQUAL(qs.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
