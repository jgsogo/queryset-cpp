
#pragma once

#include <set>
#include <bitset>
#include <iterator>
#include "utils/queryset.h"

#include "_impl/utils.h"


namespace qs {

    namespace _impl {

        template <typename Type, typename... Args>
        class FilterContainer {
            using queryset_type = typename qs::_impl::utils_queryset<Type, Args...>::type;
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

                queryset_type apply(const queryset_type& qs) const {
                    // Corner cases
                    if (_is_empty) {
                        return queryset_type();
                    }
                    if (_value_filters_apply.none()) {
                        return qs;
                    }
                    // Actual filtering
                    queryset_type result;
                    std::copy_if(qs.begin(), qs.end(), std::back_inserter(result), [=](const auto& item){
                        return this->pass(item);
                    });
                    return result;
                }

                template <typename T, typename U=T>
                void add_filter(const T& t) {
                    constexpr std::size_t index = utils::tuple::index<U, Args...>();
                    std::set<U>& t_filters = std::get<index>(_value_filters);
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

                template <typename T, typename U=T>
                void add_filter(const std::vector<T>& t) {
                    // Check if there is a single filter for this type
                    constexpr std::size_t index = utils::tuple::index<U, Args...>();

                    std::vector<U> t_sorted(t.size());
                    std::partial_sort_copy(t.begin(), t.end(), t_sorted.begin(), t_sorted.end());

                    if (_value_filters_apply[index]) {
                        std::set<U>& t_filters = std::get<index>(_value_filters);
                        std::set<U> v;
                        std::set_intersection(t_sorted.begin(), t_sorted.end(),
                            t_filters.begin(), t_filters.end(),
                            std::inserter(v, v.end()));
                        _is_empty = v.empty();
                        std::get<index>(_value_filters) = v;
                    }
                    else {
                        std::set<U> v(t_sorted.begin(), t_sorted.end());
                        std::get<index>(_value_filters) = v;
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
                    constexpr std::size_t index = utils::tuple::index<T, Args...>();
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

    template <typename... Args>
    using FilterContainer = _impl::FilterContainer<void, Args...>;

    template <typename Type, typename... Args>
    using TypedFilterContainer = _impl::FilterContainer<Type, Args...>;

}
