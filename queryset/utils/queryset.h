
#pragma once

#include <vector>
#include <algorithm>
#include "tuple.h"

namespace utils {

    // A queryset is just a vector of tuples of a given type
    template <typename... Args>
    using queryset = std::vector<std::tuple<Args...>>;

    // We can list a queryset for one of its values (it is like projecting and then flattening to a vector)
    template <typename T, typename... Args>
    std::vector<T> list(const queryset<Args...>& qs) {
        std::vector<T> result;
        std::transform(qs.begin(), qs.end(), std::back_inserter(result), [](auto& item) { 
            constexpr std::size_t index = ::utils::tuple::index<T, Args...>();
            return std::get<index>(item);
            });
        return result;
    }

    template <typename... T, typename... Args, typename = typename std::enable_if<(sizeof...(T) > 1), bool>::type>
    queryset<T...> project(const queryset<Args...>& qs) {
        queryset<T...> result;
        for (auto& item : qs) {
            typename std::tuple<T...> tup = std::make_tuple(std::get<::utils::tuple::index<T, Args...>()>(item)...);
            result.push_back(tup);
        }
        return result;
    }

    // Filter a queryset by one value in a column
    template <typename... Args, typename T>
    queryset<Args...> filter(const queryset<Args...>& qs, const T& filter_value) {
        queryset<Args...> result;
        std::copy_if(qs.begin(), qs.end(), 
                     std::back_inserter(result),
                     [&filter_value] (const std::tuple<Args...>& item) {
                        constexpr std::size_t index = ::utils::tuple::index<T, Args...>();
                        return std::get<index>(item) == filter_value;
                     }
        );
        return result;
    }

    // Filter a queryset by several values in a given column
    template <typename... Args, typename T>
    auto filter(const queryset<Args...>& qs, const std::vector<T>& filter_values) {
        queryset<Args...> result;
        std::copy_if(qs.begin(), qs.end(),
                     std::back_inserter(result),
                     [&filter_values](const std::tuple<Args...>& item) {
                        constexpr std::size_t index = ::utils::tuple::index<T, Args...>();
                        return std::find(filter_values.begin(), filter_values.end(), std::get<index>(item)) != filter_values.end();
                     }
        );
        return result;
    }

    // Filter queryset by different type values
    template <typename... Args, typename... T>
    auto filter(const queryset<Args...>& qs, const std::tuple<T...>& filter_values) {
        queryset<Args...> result;
        std::copy_if(qs.begin(), qs.end(),
                     std::back_inserter(result),
                     [&filter_values](const std::tuple<Args...>& item) {
                        return tuple::pair_compare(filter_values, item);
                     }
        );
        return result;
    }



    // Marginalize a queryset over a set of column
    template <typename T, typename... Args>
    auto marginalize(const queryset<Args...>& qs) {
        constexpr std::size_t index = ::utils::tuple::index<T, Args...>();
        typedef typename ::utils::tuple::remove_ith_type<index, std::tuple<Args...>>::type result_tuple;
        typedef typename ::utils::tuple::gen_seq<sizeof...(Args), index> result_tuple_indexes;

        std::vector<result_tuple> result;
        for (auto& item : qs) {
            result.push_back(::utils::tuple::project(item, result_tuple_indexes()));
        }
        return result;
    }
}
