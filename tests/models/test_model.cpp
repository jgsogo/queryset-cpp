
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../../queryset/models/model.h"
#include "../config_tests.h"

BOOST_AUTO_TEST_SUITE(models)

BOOST_AUTO_TEST_CASE(manager_memory)
{
    typedef qs::Model<int, float> MyModel;
    MyModel m;
    auto& manager = m.objects();
    BOOST_CHECK_EQUAL(manager.all().count(), 0);
}

BOOST_AUTO_TEST_CASE(manager_file)
{
    namespace fs = boost::filesystem;
    fs::path full_path = test_data_dir / fs::path("ex_filequeryset.tsv");

    typedef qs::Model<int, std::string, float> MyModel;
    MyModel m;
    auto& manager = m.objects(full_path.string());
    BOOST_CHECK_EQUAL(manager.all().count(), 3);
}



BOOST_AUTO_TEST_SUITE_END()
