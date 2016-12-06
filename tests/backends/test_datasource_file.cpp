
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../print_helper.hpp"
#include "../../queryset/backends/filesystem.h"
#include "../config_tests.h"

typedef qs::backends::FileQueryset<int, std::string, float> myQuerySet;
typedef myQuerySet::qs_type::value_type mytuple;


BOOST_AUTO_TEST_SUITE(datasource_file)

BOOST_AUTO_TEST_CASE(basic)
{
    namespace fs = boost::filesystem;
    fs::path full_path = test_data_dir / fs::path("ex_filequeryset.tsv");

    myQuerySet fileqs(full_path.string());
    qs::FilterContainer<int, std::string, float> filters;
    auto qs = fileqs.apply(filters);

    BOOST_CHECK_EQUAL(qs.size(), 3);
}

BOOST_AUTO_TEST_SUITE_END()
