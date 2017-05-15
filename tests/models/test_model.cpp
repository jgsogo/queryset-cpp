
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../../queryset/models/model.h"
#include "../config_tests.h"
#include "../../queryset/backends/filesystem.h"

typedef qs::Model<qs::memory::Manager, int, float> MyModelMemory;
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

BOOST_AUTO_TEST_CASE(get_method_basic)
{
	MyModelFilesystem m(std::make_tuple(1, "str", 2.f));
	BOOST_CHECK_EQUAL(m.get<0>(), m.get<int>());
	BOOST_CHECK_EQUAL(m.get<1>(), m.get<std::string>());
	BOOST_CHECK_EQUAL(m.get<2>(), m.get<float>());
}

BOOST_AUTO_TEST_CASE(get_method_fail)
{
	typedef qs::Model<qs::memory::Manager, int, float, int> MyModelMemory2;
	MyModelMemory2 a;
	//a.get<int>(); // TODO: Can I check for static_assert?
}

BOOST_AUTO_TEST_SUITE_END()
