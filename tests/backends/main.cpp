
#define BOOST_TEST_MODULE "Test backends"
#include <boost/test/unit_test.hpp>
#include "spdlog/spdlog.h"
#include <iostream>

struct GlobalConfig {
    GlobalConfig() {
        std::cout << "global setup\n";
        auto console = spdlog::stdout_logger_mt("qs");
        console->set_level(spdlog::level::debug);
        console->info("Logging queryset-cpp: test backends");
    }

    ~GlobalConfig() {
        std::cout << "global teardown\n";
    }
};

BOOST_GLOBAL_FIXTURE(GlobalConfig);