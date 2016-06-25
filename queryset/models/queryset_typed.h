
#pragma once

#include "spdlog/spdlog.h"

#include "../queryset.h"

namespace qs {
    
    template <typename Type, typename... Args>
    class TypedQuerySet : public QuerySet<Args...> {
        using BaseQs = QuerySet<Args...>;
        public:
            TypedQuerySet(const utils::ImplDataSource<Args...>& datasource) : BaseQs(datasource) {}
            TypedQuerySet(const BaseQs& other) : BaseQs(other) {}

            const Type get(const typename std::tuple_element<0, std::tuple<Args...>>::type& t) {
                return Type(BaseQs::get(t));
            }
    };

}