
#pragma once

#include "spdlog/spdlog.h"

#include "utils/queryset.h"
#include "datasource.h"
#include "filters.h"
#include "group_by.h"


namespace {
    template <typename... Args>
    class BaseQuerySet {
        public:
            BaseQuerySet(const utils::ImplDataSource<Args...>& datasource) : _datasource(datasource), _evaluated(false) {
                SPDLOG_DEBUG(spdlog::get("qs"), "BaseQuerySet<Args...>::BaseQuerySet(datasource)");
            };
            BaseQuerySet(const BaseQuerySet& other) : _datasource(other._datasource),
                                                      _filters(other._filters),
                                                      _evaluated(false) {
                SPDLOG_DEBUG(spdlog::get("qs"), "BaseQuerySet<Args...>::BaseQuerySet(BaseQuerySet other)");
            }

            bool empty() const {
                SPDLOG_DEBUG(spdlog::get("qs"), "BaseQuerySet[{}]::empty", (void*)this);
                if (!_evaluated) {
                    this->eval();
                }
                return _qs.empty();
            }

            virtual bool is_evaluated() const {
                SPDLOG_DEBUG(spdlog::get("qs"), "BaseQuerySet[{}]::is_evaluated", (void*)this);
                return _evaluated;
            }
            virtual void reset() {_evaluated = false;}

        protected:
            // Eval (cache) queryset (make it protected?)
            const utils::queryset<Args...>& eval() const {
                SPDLOG_DEBUG(spdlog::get("qs"), "BaseQuerySet[{}]::eval", (void*)this);
                if (!_evaluated) {
                    SPDLOG_DEBUG(spdlog::get("qs"), "BaseQuerySet[{}]::eval -- do evaluation", (void*)this);
                    _qs = _datasource.apply(_filters);
                    _evaluated = true;
                }
                return _qs;
            }

        protected:
            utils::FilterContainer<Args...> _filters;

        private:
            const utils::ImplDataSource<Args...>& _datasource;
            mutable utils::queryset<Args...> _qs;
            mutable bool _evaluated;
    };

    template<typename R, typename... Args>
    class BaseReturnQuerySet : public BaseQuerySet<Args...> {
        protected:
            using BaseQs = BaseQuerySet<Args...>;
        public:
            BaseReturnQuerySet(const utils::ImplDataSource<Args...>& datasource) : BaseQs(datasource) {
                SPDLOG_DEBUG(spdlog::get("qs"), "BaseReturnQuerySet<R, Args...>::BaseReturnQuerySet(datasource)");
            };
            BaseReturnQuerySet(const BaseReturnQuerySet& other) : BaseQs(other) {
                SPDLOG_DEBUG(spdlog::get("qs"), "BaseReturnQuerySet<R, Args...>::BaseReturnQuerySet(BaseReturnQuerySet other)");
            }
            BaseReturnQuerySet(const BaseQs& other) : BaseQs(other) {
                SPDLOG_DEBUG(spdlog::get("qs"), "BaseReturnQuerySet<R, Args...>::BaseReturnQuerySet(BaseQs other)");
            }

            // Filtering functions
            template<typename T>
            R& filter(const T &filter_value) {
                SPDLOG_DEBUG(spdlog::get("qs"), "BaseReturnQuerySet[{}]::filter by {}={}.", (void*)this, typeid(filter_value).name(), filter_value);
                assert(!BaseQs::is_evaluated());
                BaseQs::_filters.add_filter(filter_value);
                return static_cast<R&>(*this);
            }

            template<typename T>
            R& filter(const std::vector<T> &filter_value) {
                SPDLOG_DEBUG(spdlog::get("qs"), "BaseReturnQuerySet[{}]::filter by {}={}.", (void*)this, typeid(filter_value).name(), filter_value);
                assert(!BaseQs::is_evaluated());
                BaseQs::_filters.add_filter(filter_value);
                return static_cast<R&>(*this);
            }

            template<typename... T>
            R& filter(const std::tuple<T...> &filter_value) {
                SPDLOG_DEBUG(spdlog::get("qs"), "BaseReturnQuerySet[{}]::filter by {}.", (void*)this, typeid(filter_value).name());
                assert(!BaseQs::is_evaluated());
                BaseQs::_filters.add_filter(filter_value);
                return static_cast<R&>(*this);
            }
    };
}

template <typename... Args>
class QuerySet;

template <typename T, typename... Args>
class GroupedQuerySet : public BaseReturnQuerySet<GroupedQuerySet<T, Args...>, Args...> {
        using BaseQs = BaseReturnQuerySet<GroupedQuerySet<T, Args...>, Args...>;
    public:
        GroupedQuerySet(const utils::ImplDataSource<Args...>& datasource) : BaseQs(datasource), _evaluated(false) {}
        //GroupedQuerySet(const BaseQs& other) : BaseQs(other), _evaluated(false) {}
        GroupedQuerySet(const BaseQuerySet<Args...>& other) : BaseQs(other), _evaluated(false) {}
        GroupedQuerySet(const GroupedQuerySet<T, Args...>& other) : BaseQs(other),
                                                                    _evaluated(false) {}

        const std::map<T, QuerySet<Args...>>& get() const {
            SPDLOG_DEBUG(spdlog::get("qs"), "GroupedQuerySet[{}]::get", (void*)this);
            return this->eval();
        }

        virtual bool is_evaluated() const {
            SPDLOG_DEBUG(spdlog::get("qs"), "GroupedQuerySet[{}]::is_evaluated", (void*)this);
            return _evaluated;
        }
        virtual void reset() {_evaluated = false;}

    protected:
        // Eval (cache) queryset (make it protected?)
        const std::map<T, QuerySet<Args...>>& eval() const {
            SPDLOG_DEBUG(spdlog::get("qs"), "GroupedQuerySet[{}]::eval", (void*)this);
            if (!_evaluated) {
                SPDLOG_DEBUG(spdlog::get("qs"), "GroupedQuerySet[{}]::eval -- do evaluation", (void*)this);
                auto qs = this->BaseQs::eval();
                auto values = utils::list<T>(qs);
                for (auto& v: values) {
                    _qs.insert(std::make_pair(v, QuerySet<Args...>(*this).filter(v)));
                }
                _evaluated = true;
            }
            return _qs;
        }

    private:
        mutable std::map<T, QuerySet<Args...>> _qs;
        mutable bool _evaluated;
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
            SPDLOG_DEBUG(spdlog::get("qs"), "QuerySet<Args...>::QuerySet(datasource)");
        }

        QuerySet(const BaseQs& other) : BaseQs(other) {
            SPDLOG_DEBUG(spdlog::get("qs"), "QuerySet<Args...>::QuerySet(other)");
        }

        const utils::queryset<Args...>& get() const {
            SPDLOG_DEBUG(spdlog::get("qs"), "QuerySet[{}]::get", (void*)this);
            return this->eval();
        }

        std::size_t count() const {
            SPDLOG_DEBUG(spdlog::get("qs"), "QuerySet[{}]::count", (void*)this);
            return this->get().size();
        }

        // Grouping by field types
        template <typename T>
        GroupedQuerySet<T, Args...> groupBy() {
            SPDLOG_DEBUG(spdlog::get("qs"), "QuerySet[{}]::groupBy", (void*)this);
            assert (!BaseQs::is_evaluated());
            return GroupedQuerySet<T, Args...>(*this);
        }

#ifdef WIP
        template <typename... Ts, typename = typename std::enable_if<(sizeof...(Ts) > 1), bool>::type>
        GroupedMultipleQuerySet<std::tuple<Ts...>, Args...> groupBy() {
            return GroupedMultipleQuerySet<std::tuple<Ts...>, Args...>(*this);
        }
#endif
};

