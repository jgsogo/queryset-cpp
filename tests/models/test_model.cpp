
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../../queryset/models/model.h"
#include "../config_tests.h"
#include "../../queryset/backends/filesystem.h"

typedef qs::Model<qs::manager::MemoryManager, int, float> MyModelMemory;
typedef qs::Model <qs::filesystem::Manager, int, std::string, float > MyModelFilesystem;
const std::string qs::filesystem::Manager<MyModelFilesystem>::_filename = (test_data_dir / boost::filesystem::path("ex_filequeryset.tsv")).string();

BOOST_AUTO_TEST_SUITE(models)

BOOST_AUTO_TEST_CASE(manager_memory)
{
	BOOST_CHECK_EQUAL(MyModelMemory::objects().all().count(), 0);

	MyModelMemory m;
	auto& manager = m.objects();
	BOOST_CHECK_EQUAL(manager.all().count(), 0);
}

BOOST_AUTO_TEST_CASE(manager_file)
{
	namespace fs = boost::filesystem;
	fs::path full_path = test_data_dir / fs::path("ex_filequeryset.tsv");
	BOOST_CHECK_EQUAL(full_path, MyModelFilesystem::objects().filename());
	BOOST_CHECK_EQUAL(MyModelFilesystem::objects().all().count(), 3);

    MyModelFilesystem m;
    auto& manager = m.objects();
    BOOST_CHECK_EQUAL(manager.all().count(), 3);
}

BOOST_AUTO_TEST_SUITE_END()
