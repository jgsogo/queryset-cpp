
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../../queryset/models/model.h"
#include "../config_tests.h"


class MyModel;

class MyModelManager : public qs::FileManager<MyModel, int, std::string, float> {
    using BaseManager = qs::FileManager<MyModel, int, std::string, float>;
    public:
        MyModelManager(const std::string& filename) : BaseManager(filename) {};
        virtual ~MyModelManager() {};

        virtual BaseManager::QuerySet all() const {
            return BaseManager::all().filter<int>(1);
        }
};

class MyModel : public qs::BaseModel<MyModel, int, std::string, float> {
    using BaseModel = qs::BaseModel<MyModel, int, std::string, float>;
    public:
        MyModel() : BaseModel() {}
        MyModel(const BaseModel::tuple& t) : BaseModel(t) {}

        std::size_t product() {
            return 12;
        }

        // with a custom manager
        static MyModelManager& objects(const std::string& filename) {
            static MyModelManager manager(filename);
            return manager;
        }

};

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

/*
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
typedef qs::BaseModel<MyModelManager> MyModel;


BOOST_AUTO_TEST_SUITE(model_custom)

BOOST_AUTO_TEST_CASE(count)
{
    BOOST_CHECK_EQUAL(MyModel::objects().all().count(), 3);
    MyModel m;
    auto& manager = m.objects();
    BOOST_CHECK_EQUAL(manager.all().count(), 3);
}

BOOST_AUTO_TEST_CASE(parse)
{
    BOOST_CHECK_EQUAL(MyModel::objects().all().count(), 3);
    MyModel m;
    auto& manager = m.objects();
    BOOST_CHECK_EQUAL(manager.all().count(), 3);

    auto item0 = manager.all()[0];
    BOOST_CHECK_EQUAL(std::get<0>(item0), 1);
    BOOST_CHECK_EQUAL(std::get<1>(item0), "hola");
    BOOST_CHECK_EQUAL(std::get<2>(item0), 1.f);
}

BOOST_AUTO_TEST_CASE(other_file)
{
    BOOST_CHECK_THROW(MyModel::objects("not_exist.file").all().count(), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()
*/
