
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../../queryset/models/model.h"
#include "../config_tests.h"

class MyModelManager;

class MyModel : public qs::BaseModel<MyModel, int, std::string, float> {
    using BaseModel = qs::BaseModel<MyModel, int, std::string, float>;
    public:
        MyModel() : BaseModel() {}
        MyModel(const BaseModel::tuple& t) : BaseModel(t) {}

        std::size_t product() {
            return 12;
        }

        // with a custom manager
        static MyModelManager& objects(const std::string& filename);
};

class MyModelManager : public qs::FileManager<MyModel> {
    using BaseManager = qs::FileManager<MyModel>;
    public:
        MyModelManager(const std::string& filename) : BaseManager(filename) {};
        virtual ~MyModelManager() {};

        virtual BaseManager::QuerySet all() const {
            return BaseManager::all().filter<int>(1);
        }
};

MyModelManager& MyModel::objects(const std::string& filename) {
    static MyModelManager manager(filename);
    return manager;
}


BOOST_AUTO_TEST_SUITE(model_custom_manager)

BOOST_AUTO_TEST_CASE(count)
{
    namespace fs = boost::filesystem;
    fs::path full_path = test_data_dir / fs::path("ex_filequeryset.tsv");

    BOOST_CHECK_EQUAL(MyModel::objects(full_path.string()).all().count(), 1);
}

BOOST_AUTO_TEST_CASE(custom_method)
{
    namespace fs = boost::filesystem;
    fs::path full_path = test_data_dir / fs::path("ex_filequeryset.tsv");

    auto item1 = MyModel::objects(full_path.string()).all().get(1);
    BOOST_CHECK_EQUAL(item1.pk(), 1);
    BOOST_CHECK_EQUAL(item1.product(), 12);
}

BOOST_AUTO_TEST_SUITE_END()

