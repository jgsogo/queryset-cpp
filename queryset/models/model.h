
#pragma once

#include <type_traits>

#include "spdlog/spdlog.h"
#include "manager.h"
#include <iostream>


namespace qs {

    template <typename TManager>
    class BaseModel {
        public:
            BaseModel() : _evaluated(false) {};
            BaseModel(const typename TManager::tuple& data) : _data(data), _evaluated(true) {};
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

            typename TManager::pk pk() const {
                return std::get<0>(_data);
            }

            virtual void print(std::ostream& os) const {
                os << BaseModel<TManager>::name() << "[" << pk() << "]";
            }
        protected:
            void eval() const {
                if (!_evaluated) {
                    TManager::tuple data = BaseModel<TManager>::objects().all().get(pk());
                    assert(std::get<0>(data) == pk());
                    _data = data;
                    _evaluated = true;
                }
            }

            friend bool operator==(const BaseModel<TManager>& lhs, const BaseModel<TManager>& rhs) {
                return lhs.pk() == rhs.pk();
            }

            friend bool operator<(const BaseModel<TManager>& lhs, const BaseModel<TManager>& rhs) {
                return lhs.pk() < rhs.pk();
            }

            // assign value
            friend std::istream& operator >> (std::istream & lhs, BaseModel<TManager>& rhs)
            {
                // Read just one value: primary key
                lhs >> std::get<0>(rhs._data);
                return lhs;
            }
        protected:
            mutable typename TManager::tuple _data;
            mutable bool _evaluated;
    };

    template <typename pk, typename... Args>
    using Model = BaseModel<Manager<void, pk, Args...>>;

}

template<class Ch, class Tr, typename TManager>
std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& os, const qs::BaseModel<TManager>& rhs) {
    rhs.print(os);
    return os;
}
