
#pragma once

#include "spdlog/spdlog.h"
#include "manager.h"


namespace qs {

    template <typename... Args>
    class Model {
        public:
            Model() {};
            Model(const std::tuple<Args...>& data) : _data(data) {};

            template <typename T>
            static BaseManager<Args...>& objects(const T& t) {
                SPDLOG_DEBUG(spdlog::get("qs"), "Model::objects<T>(t={})", t);
                static Manager<T, Args...> manager(t);
                return manager;
            }
            static BaseManager<Args...>& objects() {
                SPDLOG_DEBUG(spdlog::get("qs"), "Model::objects()");
                static Manager<void, Args...> manager;
                return manager;
            }
        protected:
            std::tuple<Args...> _data;
    };

}
