
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../../queryset/models/model.h"
#include "../config_tests.h"
#include "../../queryset/backends/filesystem.h"

BOOST_AUTO_TEST_SUITE(models)


BOOST_AUTO_TEST_CASE(manager_default)
{
    typedef qs::Model<qs::manager::MemoryManager, int, float> MyModel;
    BOOST_CHECK_EQUAL(MyModel::objects().all().count(), 0);

    MyModel m;
    auto& manager = m.objects();
    BOOST_CHECK_EQUAL(manager.all().count(), 0);
}

BOOST_AUTO_TEST_CASE(manager_memory)
{
    //typedef qs::MemoryManager<int, float> MyManager;
    typedef qs::Model<qs::manager::MemoryManager, int, float> MyModel;
    BOOST_CHECK_EQUAL(MyModel::objects().all().count(), 0);
    
    MyModel m;
    auto& manager = m.objects();
    BOOST_CHECK_EQUAL(manager.all().count(), 0);
}

BOOST_AUTO_TEST_CASE(manager_file)
{
    typedef qs::Model<qs::manager::FileManager, int, std::string, float> MyModel;
	namespace fs = boost::filesystem;
	fs::path full_path = test_data_dir / fs::path("ex_filequeryset.tsv");
    BOOST_CHECK_EQUAL(MyModel::objects(full_path.string()).all().count(), 3);

    //MyModel m;
    //auto& manager = m.objects();
    //BOOST_CHECK_EQUAL(manager.all().count(), 3);
}

BOOST_AUTO_TEST_SUITE_END()
