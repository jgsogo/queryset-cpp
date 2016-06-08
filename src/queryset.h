
#pragma once

#include "utils/queryset.h"
#include "datasource.h"
#include "filters.h"
#include "group_by.h"


namespace {
    template <typename... Args>
    class BaseQuerySet {
        public:
            BaseQuerySet(const utils::ImplDataSource<Args...>& datasource) : _datasource(datasource) { };
            BaseQuerySet(const BaseQuerySet& other) : _datasource(other._datasource), _filters(other._filters) { }

            bool empty() const {
                return _filters.empty();
            }

        protected:
            const utils::ImplDataSource<Args...>& _datasource;
            utils::FilterContainer<Args...> _filters;
    };

    template<typename R, typename... Args>
    class BaseReturnQuerySet : public BaseQuerySet<Args...> {
        protected:
            using BaseQs = BaseQuerySet<Args...>;
        public:
            BaseReturnQuerySet(const utils::ImplDataSource<Args...>& datasource) : BaseQs(datasource) { };
            BaseReturnQuerySet(const BaseReturnQuerySet& other) : BaseQs(other) { }
            BaseReturnQuerySet(const BaseQs& other) : BaseQs(other) { }

            // Filtering functions
            template<typename T>
            R filter(const T &filter_value) {
                BaseQs::_filters.add_filter(filter_value);
                return *this;
            }

            template<typename T>
            R filter(const std::vector<T> &filter_value) {
                BaseQs::_filters.add_filter(filter_value);
                return *this;
            }

            template<typename... T>
            R filter(const std::tuple<T...> &filter_value) {
                BaseQs::_filters.add_filter(filter_value);
                return *this;
            }
    };
}

template <typename... Args>
class QuerySet;

template <typename T, typename... Args>
class GroupedQuerySet : public BaseReturnQuerySet<GroupedQuerySet<T, Args...>, Args...> {
        using BaseQs = BaseReturnQuerySet<GroupedQuerySet<T, Args...>, Args...>;
    public:
        GroupedQuerySet(const utils::ImplDataSource<Args...>& datasource) : BaseQs(datasource) {}
        GroupedQuerySet(const BaseQs& other) : BaseQs(other) {}
        GroupedQuerySet(const BaseQuerySet<Args...>& other) : BaseQs(other) {}

        std::map<T, QuerySet<Args...>> get() {
            // Put all with the same value of T into a group
            auto qs = BaseQs::_datasource.apply(BaseQs::_filters);
            auto values = utils::list<T>(qs);
            std::map<T, QuerySet<Args...>> result;
            for (auto& v: values) {
                result.insert(std::make_pair(v, QuerySet<Args...>(*this).filter(v)));
            }
            return result;
        }
};

#ifdef WIP
template <typename T, typename... Args>
class GroupedMultipleQuerySet : public BaseReturnQuerySet<GroupedMultipleQuerySet<T, Args...>, Args...> {
    protected:
        using BaseQs = BaseReturnQuerySet<GroupedMultipleQuerySet<T, Args...>, Args...>;
    public:
        GroupedMultipleQuerySet(const utils::ImplDataSource<Args...>& datasource) : BaseQs(datasource) {}
        GroupedMultipleQuerySet(const BaseQs& other) : BaseQs(other) {}
        GroupedMultipleQuerySet(const BaseQuerySet<Args...>& other) : BaseQs(other) {}

        std::map<T, utils::queryset<Args...>> get() {
            // Put all with the same values of T... into a group
            constexpr std::make_index_sequence<std::tuple_size<T>::value> indexes();
            //return utils::groupBy<std::tuple_element_t<indexes, T> >(BaseQs::_db, BaseQs::_filters);
            return utils::groupBy<std::tuple_type<T, indexes>::type >(BaseQs::_db, BaseQs::_filters);
        }
};
#endif


template <typename... Args>
class QuerySet : public BaseReturnQuerySet<QuerySet<Args...>, Args...> {
        using BaseQs = BaseReturnQuerySet<QuerySet<Args...>, Args...>;
    public:
        QuerySet(const utils::ImplDataSource<Args...>& datasource) : BaseQs(datasource) {
        }

        QuerySet(const BaseQs& other) : BaseQs(other) {}

        utils::queryset<Args...> get() const {
            return BaseQs::_datasource.apply(BaseQs::_filters);
        }

        // Grouping by field types
        template <typename T>
        GroupedQuerySet<T, Args...> groupBy() {
            return GroupedQuerySet<T, Args...>(*this);
        }

#ifdef WIP
        template <typename... Ts, typename = typename std::enable_if<(sizeof...(Ts) > 1), bool>::type>
        GroupedMultipleQuerySet<std::tuple<Ts...>, Args...> groupBy() {
            return GroupedMultipleQuerySet<std::tuple<Ts...>, Args...>(*this);
        }
#endif
};
