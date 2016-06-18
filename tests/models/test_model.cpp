
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../../queryset/models/model.h"
#include "../config_tests.h"

BOOST_AUTO_TEST_SUITE(models)


BOOST_AUTO_TEST_CASE(manager_default)
{
    typedef qs::Model<int, float> MyModel;
    BOOST_CHECK_EQUAL(MyModel::objects().all().count(), 0);

    MyModel m;
    auto& manager = m.objects();
    BOOST_CHECK_EQUAL(manager.all().count(), 0);
}

BOOST_AUTO_TEST_CASE(manager_memory)
{
    typedef qs::MemoryManager<int, float> MyManager;
    typedef qs::BaseModel<MyManager, int, float> MyModel;
    BOOST_CHECK_EQUAL(MyModel::objects().all().count(), 0);
    
    MyModel m;
    auto& manager = m.objects();
    BOOST_CHECK_EQUAL(manager.all().count(), 0);
}

BOOST_AUTO_TEST_CASE(manager_file)
{
    namespace fs = boost::filesystem;
    fs::path full_path = test_data_dir / fs::path("ex_filequeryset.tsv");

    typedef qs::FileManager<int, std::string, float> MyManager;
    typedef qs::BaseModel<MyManager, int, std::string, float> MyModel;
    BOOST_CHECK_EQUAL(MyModel::objects(full_path.string()).all().count(), 3);

    MyModel m;
    auto& manager = m.objects(full_path.string());
    BOOST_CHECK_EQUAL(manager.all().count(), 3);
}

BOOST_AUTO_TEST_SUITE_END()
