
#pragma once

#include <type_traits>

#include "spdlog/spdlog.h"
#include "manager.h"


namespace qs {

    template <typename TManager>
    class BaseModel {
        public:
            BaseModel() {};
            BaseModel(const typename TManager::tuple& data) : _data(data) {};

            static std::string name() { BaseModel m; return typeid(m).name(); }

            static TManager& objects() {
                //static_assert(std::is_base_of<BaseManager<Args...>, TManager>::value, "First template argument to qs::Model must be the model itself.");
                static TManager manager;
                return manager;
            }

            template <typename... T>
            static TManager& objects(T... t) {
                //static_assert(std::is_base_of<BaseManager<Args...>, TManager>::value, "First template argument to qs::Model must be the model itself.");
                static TManager manager(t...);
                return manager;
            }

        protected:
            typename TManager::tuple _data;
    };

    template <typename... Args>
    using Model = BaseModel<Manager<void, Args...>>;

}
