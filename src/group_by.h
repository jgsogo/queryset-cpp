
#pragma once

#include <map>
#include "utils/queryset.h"
#include "filters.h"


namespace utils {

    template <typename T, typename... Args>
    std::map<T, queryset<Args...>> groupBy(const queryset<Args...>& qs, const utils::FilterContainer<Args...>& filters) {
        typename std::map<T, queryset<Args...>> result;
        constexpr std::size_t index = tuple::index<T, Args...>();
        for (auto& item: qs) {
            if (filters.pass(item)) {
                result[std::get<index>(item)].push_back(item);
            }
        }
        return result;
    }

    template <typename... T, typename... Args, typename = typename std::enable_if<(sizeof...(T) >1), bool>::type>
    std::map<std::tuple<T...>, queryset<Args...>> groupBy(const queryset<Args...>& qs, const utils::FilterContainer<Args...>& filters) {
        typename std::map<std::tuple<T...>, queryset<Args...>> result;
        for (auto& item: qs) {
            if (filters.pass(item)) {
                typename std::tuple<T...> tup = std::make_tuple(std::get<::utils::tuple::index<T, Args...>()>(item)...);
                result[tup].push_back(item);
                }
        }
        return result;
    }


    template <typename... Args>
    class GroupBy {
        public:

    };

}