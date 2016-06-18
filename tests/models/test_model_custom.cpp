
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../../queryset/models/model.h"
#include "../config_tests.h"


class MyModelManager : public qs::FileManager<int, std::string, float> {
    public:
        static std::string filename() {
            namespace fs = boost::filesystem;
            fs::path full_path = test_data_dir / fs::path("ex_filequeryset.tsv");
            return full_path.string();
        };
        MyModelManager() : qs::FileManager<int, std::string, float>(filename()) {};
        MyModelManager(const std::string& filename) : qs::FileManager<int, std::string, float>(filename) {};
};
typedef qs::BaseModel<MyModelManager, int, std::string, float> MyModel;


BOOST_AUTO_TEST_SUITE(model_custom)

BOOST_AUTO_TEST_CASE(count)
{
    BOOST_CHECK_EQUAL(MyModel::objects().all().count(), 3);
    MyModel m;
    auto& manager = m.objects();
    BOOST_CHECK_EQUAL(manager.all().count(), 3);
}

BOOST_AUTO_TEST_CASE(other_file)
{
    BOOST_CHECK_THROW(MyModel::objects("not_exist.file").all().count(), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()
