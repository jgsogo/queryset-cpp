
#pragma once

#include <set>
#include <bitset>
#include <iterator>
#include "utils/queryset.h"

namespace utils {

    template <typename... Args>
    class FilterContainer {
        public:
            FilterContainer() : _is_empty(false) {}
            ~FilterContainer() {}

            bool empty() const {
                return _is_empty;
            }

            bool pass(const std::tuple<Args...>& t) const {
                bool pass = true;
                ::utils::tuple::for_each(_value_filters, [this, &t, &pass](const auto& values){
                    pass &= FilterContainer::filter_pass(values, _value_filters_apply, t);
                });
                return pass;
            }

            queryset<Args...> apply(const queryset<Args...>& qs) const {
                // Corner cases
                if (_is_empty) {
                    return queryset<Args...>();
                }
                if (_value_filters_apply.none()) {
                    return qs;
                }
                // Actual filtering
                queryset<Args...> result;
                std::copy_if(qs.begin(), qs.end(), std::back_inserter(result), [=](const auto& item){
                    return this->pass(item);
                });
                return result;
            }

            template <typename T>
            void add_filter(const T& t) {
                constexpr std::size_t index = tuple::index<T, Args...>();
                std::set<T>& t_filters = std::get<index>(_value_filters);
                if (_value_filters_apply[index]) {
                    _is_empty = std::find(t_filters.begin(), t_filters.end(), t) == t_filters.end();
                    if (!_is_empty) {
                        t_filters.clear();
                        t_filters.insert(t);
                    }
                }
                else {
                    _value_filters_apply[index] = true;
                    t_filters.insert(t);
                }
            }

            template <typename T>
            void add_filter(const std::set<T>& t) {
                // Check if there is a single filter for this type
                constexpr std::size_t index = tuple::index<T, Args...>();
                std::sort(t.begin(), t.end());
                if (_value_filters_apply[index]) {
                    std::set<T>& t_filters = std::get<index>(_value_filters);
                    typename std::set<T> v;
                    typename std::set<T>::iterator it;
                    it = std::set_difference(t.begin(), t.end(), t_filters.begin(), t_filters.end(), v.begin());
                    v.resize(it - v.begin());

                    _is_empty = v.empty();
                    _value_filters[index] = v;
                }
                else {
                    _value_filters[index] = t;
                }
            }

            template <typename... T>
            void add_filter(const std::tuple<T...>& t) {
                this->add_filter(std::get<0>(t));
                this->add_filter(::utils::tuple::tail(t));
            }

            template <typename T>
            void add_filter(const std::tuple<T>& t) {
                this->add_filter(std::get<0>(t));
            }

        protected:
            template <class T>
            static bool filter_pass(const std::set<T>& values, const std::bitset<sizeof...(Args)>& values_apply, const std::tuple<Args...>& t) {
                constexpr std::size_t index = tuple::index<T, Args...>();
                if (values_apply[index]) {
                    return std::find(values.begin(), values.end(), std::get<index>(t)) != values.end();
                }
                return true;
            }

        protected:
            bool _is_empty;
            /*
            std::tuple<Args...> _single_value_filters;
            std::bitset<sizeof...(Args)> _single_value_filters_apply;
            */
            std::tuple<std::set<Args>...> _value_filters;
            std::bitset<sizeof...(Args)> _value_filters_apply;
    };
    
}
