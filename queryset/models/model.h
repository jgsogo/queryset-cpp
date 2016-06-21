
#pragma once

#include <type_traits>

#include "spdlog/spdlog.h"
#include "manager.h"
#include <iostream>


namespace qs {

    template <typename TManager>
    class BaseModel {
        public:
            BaseModel() {};
            BaseModel(const typename TManager::tuple& data) : _data(data) {};
            virtual ~BaseModel() {};

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

            virtual void serialize(std::ostream& os) const {
                os << _data;
            }
        protected:
            friend bool operator==(const BaseModel<TManager>& lhs, const BaseModel<TManager>& rhs) {
                return lhs._data == rhs._data;
            }

            friend bool operator<(const BaseModel<TManager>& lhs, const BaseModel<TManager>& rhs) {
                return lhs._data < rhs._data;
            }

            // assign value
            friend std::istream& operator >> (std::istream & lhs, BaseModel<TManager>& rhs)
            {
                // Read just one value, tipically primary key
                lhs >> std::get<0>(rhs._data);
                return lhs;
            }
        protected:
            typename TManager::tuple _data;
    };

    template <typename... Args>
    using Model = BaseModel<Manager<void, Args...>>;

}

template<class Ch, class Tr, typename TManager>
std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& os, const qs::BaseModel<TManager>& rhs) {
    rhs.serialize(os);
    return os;
}
