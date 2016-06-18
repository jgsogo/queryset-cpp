
#pragma once

#include <type_traits>

#include "spdlog/spdlog.h"
#include "manager.h"


namespace qs {

    template <typename TManager = Manager<void, Args...>, typename... Args>
    class BaseModel {
        public:
            BaseModel() {};
            BaseModel(const std::tuple<Args...>& data) : _data(data) {};

            static std::string name() { BaseModel m; return typeid(m).name(); }

            static BaseManager<Args...>& objects() {
                static_assert(std::is_base_of<BaseManager<Args...>, TManager>::value, "First template argument to qs::Model must be the model itself.");
                static TManager manager;
                return manager;
            }

            template <typename... T>
            static BaseManager<Args...>& objects(T... t) {
                static_assert(std::is_base_of<BaseManager<Args...>, TManager>::value, "First template argument to qs::Model must be the model itself.");
                static TManager manager(t...);
                return manager;
            }

        protected:
            std::tuple<Args...> _data;
    };

    template <typename... Args>
    using Model = BaseModel<Manager<void, Args...>, Args...>;

}
