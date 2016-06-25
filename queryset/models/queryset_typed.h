
#pragma once

#include "spdlog/spdlog.h"

#include "../queryset.h"

namespace qs {
    
    template <typename Type, typename... Args>
    class TypedQuerySet;

    template <typename Type, typename T, typename... Args>
    class GroupedTypedQuerySet : public GroupedQuerySet<T, Args...> {
        using BaseQs = GroupedQuerySet<T, Args...>;
    public:
        GroupedTypedQuerySet(const utils::ImplDataSource<Args...>& datasource) : BaseQs(datasource) {}
        //GroupedQuerySet(const BaseQs& other) : BaseQs(other), _evaluated(false) {}
        GroupedTypedQuerySet(const BaseQuerySet<Args...>& other) : BaseQs(other) {}
        GroupedTypedQuerySet(const GroupedQuerySet<T, Args...>& other) : BaseQs(other) {}

        const TypedQuerySet<Type, Args...>& at(const T& t) const {
            return this->eval().at(t);
        }
    };


    template <typename Type, typename... Args>
    class TypedQuerySet : public QuerySet<Args...> {
        using BaseQs = QuerySet<Args...>;
        public:
            TypedQuerySet(const utils::ImplDataSource<Args...>& datasource) : BaseQs(datasource) {}
            TypedQuerySet(const BaseQs& other) : BaseQs(other) {}

            const Type get(const typename std::tuple_element<0, std::tuple<Args...>>::type& t) {
                return Type(BaseQs::get(t));
            }

            // Grouping by field types
            template <typename T>
            GroupedTypedQuerySet<Type, T, Args...> groupBy() {
                assert(!BaseQs::is_evaluated());
                return GroupedTypedQuerySet<Type, T, Args...>(*this);
            }
    };


}