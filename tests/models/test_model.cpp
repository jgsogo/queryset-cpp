
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
    //typedef qs::MemoryManager<int, float> MyManager;
    typedef qs::Model<int, float> MyModel;
    BOOST_CHECK_EQUAL(MyModel::objects().all().count(), 0);
    
    MyModel m;
    auto& manager = m.objects();
    BOOST_CHECK_EQUAL(manager.all().count(), 0);
}

typedef qs::Model<int, std::string, float> MyModel;
template <> qs::BaseManager<MyModel>& qs::Model<MyModel, int, std::string, float>::objects() {
	namespace fs = boost::filesystem;
	fs::path full_path = test_data_dir / fs::path("ex_filequeryset.tsv");
	static qs::FileManager<MyModel> manager(full_path.string());
	return manager;
}

BOOST_AUTO_TEST_CASE(manager_file)
{
    BOOST_CHECK_EQUAL(MyModel::objects().all().count(), 3);

    MyModel m;
    auto& manager = m.objects();
    BOOST_CHECK_EQUAL(manager.all().count(), 3);
}

BOOST_AUTO_TEST_SUITE_END()
