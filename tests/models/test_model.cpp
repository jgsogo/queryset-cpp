
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

#include "../../queryset/models/model.h"
#include "../config_tests.h"

BOOST_AUTO_TEST_SUITE(models)

BOOST_AUTO_TEST_CASE(manager)
{
    typedef qs::Model<int, float> MyModel;
    MyModel m;

    auto& manager_file = m.objects(std::string("hola"));
    auto& manager_mem = m.objects();
}

BOOST_AUTO_TEST_SUITE_END()
