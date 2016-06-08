
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../utils/print_helper.hpp"
#include "../../src/datasource.h"
#include "../config_tests.h"

typedef ::utils::FileQueryset<int, std::string, float> myQuerySet;
typedef myQuerySet::qs_type::value_type mytuple;


BOOST_AUTO_TEST_SUITE(datasource_file)

BOOST_AUTO_TEST_CASE(basic)
{
    namespace fs = boost::filesystem;
    fs::path full_path = test_data_dir / fs::path("ex_filequeryset.tsv");

    myQuerySet fileqs(full_path.string());
    utils::FilterContainer<int, std::string, float> filters;
    auto qs = fileqs.apply(filters);

    BOOST_CHECK_EQUAL(qs.size(), 3);
}

BOOST_AUTO_TEST_SUITE_END()
