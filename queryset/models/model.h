
#pragma once

#include "manager.h"

namespace qs {

    template <typename... Args>
    class Model {
        public:
            Model();
            Model(const std::tuple<Args...>& data);

            template <typename T = void>
            static BaseManager<Args...>& objects(const T& t);
        protected:
            std::tuple<Args...> _data;
    };

    template <typename... Args>
    template <>
    BaseManager<Args...>& Model::objects<void>() {
        static Manager<Args...> manager;
        return manager
    };

    template <typename... Args>
    template <>
    BaseManager<Args...>& Model::objects<std::string>(const std::string& filename) {
        static Manager<Args..., std::string> manager(filename);
        return manager
    };
}