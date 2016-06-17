
#define BOOST_TEST_MODULE "Test queryset"
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include "spdlog/spdlog.h"

// entry point:
int main(int argc, char* argv[])
{
    auto console = spdlog::stdout_logger_mt("qs");
    console->set_level(spdlog::level::debug);
    console->info("Logging queryset-cpp: test queryset");

    return boost::unit_test_framework::unit_test_main(init_unit_test, argc, argv);
}
