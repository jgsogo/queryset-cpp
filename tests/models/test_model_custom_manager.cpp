
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../../queryset/models/model.h"
#include "../config_tests.h"
#include "../../queryset/backends/filesystem.h"


template <typename TModel>
class MyModelManager : public qs::filesystem::Manager<TModel> {
    using BaseManager = typename qs::filesystem::Manager<TModel>;
    public:
        MyModelManager() : BaseManager() {};
        virtual ~MyModelManager() {};

        virtual typename BaseManager::QuerySet all() const {
            return BaseManager::all().template filter<int>(1);
        }
};

class MyModel : public qs::BaseModel<MyModel, MyModelManager, int, std::string, float> {
    using BaseModel = qs::BaseModel<MyModel, MyModelManager, int, std::string, float>;
    public:
        MyModel() : BaseModel() {}
        MyModel(const BaseModel::tuple& t) : BaseModel(t) {}

        std::size_t product() {
            return 12;
        }
};

const std::string qs::filesystem::Manager<MyModel>::_filename = (test_data_dir / boost::filesystem::path("ex_filequeryset.tsv")).string();


BOOST_AUTO_TEST_SUITE(model_custom_manager)

BOOST_AUTO_TEST_CASE(count)
{
    BOOST_CHECK_EQUAL(MyModel::objects().all().count(), 1);
}

BOOST_AUTO_TEST_CASE(custom_method)
{
    auto item1 = MyModel::objects().all().get(1);
    BOOST_CHECK_EQUAL(item1.pk(), 1);
    BOOST_CHECK_EQUAL(item1.product(), 12);
}

BOOST_AUTO_TEST_SUITE_END()

