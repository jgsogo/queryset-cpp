
#pragma once

#include "spdlog/spdlog.h"

#include "utils/queryset.h"
#include "datasource.h"
#include "filters.h"
#include "group_by.h"
#include "exceptions.h"


namespace {
    template <typename... Args>
    class BaseQuerySet {
        public:
            BaseQuerySet(const utils::ImplDataSource<Args...>& datasource) : _datasource(datasource), _evaluated(false) {};
            BaseQuerySet(const BaseQuerySet& other) : _datasource(other._datasource),
                                                      _filters(other._filters),
                                                      _evaluated(false) {}

            bool empty() const {
                if (!_evaluated) {
                    this->eval();
                }
                return _qs.empty();
            }

            virtual bool is_evaluated() const {
                return _evaluated;
            }
            virtual void reset() {_evaluated = false;}

        protected:
            // Eval (cache) queryset (make it protected?)
            const utils::queryset<Args...>& eval() const {
                SPDLOG_DEBUG(spdlog::get("qs"), "BaseQuerySet[{}]::eval", (void*)this);
                if (!_evaluated) {
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
            BaseReturnQuerySet(const utils::ImplDataSource<Args...>& datasource) : BaseQs(datasource) {};
            BaseReturnQuerySet(const BaseReturnQuerySet& other) : BaseQs(other) {}
            BaseReturnQuerySet(const BaseQs& other) : BaseQs(other) {}

            // Filtering functions
            template<typename T>
            R& filter(const T &filter_value) {
                assert(!BaseQs::is_evaluated());
                BaseQs::_filters.add_filter(filter_value);
                return static_cast<R&>(*this);
            }

            template<typename T>
            R& filter(const std::vector<T> &filter_value) {
                assert(!BaseQs::is_evaluated());
                BaseQs::_filters.add_filter(filter_value);
                return static_cast<R&>(*this);
            }

            template<typename... T>
            R& filter(const std::tuple<T...> &filter_value) {
                assert(!BaseQs::is_evaluated());
                BaseQs::_filters.add_filter(filter_value);
                return static_cast<R&>(*this);
            }
    };
}

template <typename... Args>
class QuerySet;

template <typename T, typename... Args>
class GroupedQuerySet : public BaseQuerySet<Args...> {
        using BaseQs = BaseQuerySet<Args...>;
    public:
        GroupedQuerySet(const utils::ImplDataSource<Args...>& datasource) : BaseQs(datasource), _evaluated(false) {}
        //GroupedQuerySet(const BaseQs& other) : BaseQs(other), _evaluated(false) {}
        GroupedQuerySet(const BaseQuerySet<Args...>& other) : BaseQs(other), _evaluated(false) {}
        GroupedQuerySet(const GroupedQuerySet<T, Args...>& other) : BaseQs(other),
                                                                    _evaluated(false) {}
        
        std::size_t count() const {
            return this->eval().size();
        }

        virtual bool is_evaluated() const {
            return _evaluated;
        }

        virtual void reset() {_evaluated = false;}

        typename const QuerySet<Args...>& at(const T& t) const {
            return this->eval().at(t);
        }

        typename std::map<T, QuerySet<Args...>>::const_iterator begin() const {
            return this->eval().begin();
        }

        typename std::map<T, QuerySet<Args...>>::const_iterator end() const {
            return this->eval().end();
        }

        typename std::map<T, QuerySet<Args...>>::const_reverse_iterator rbegin() const {
            return this->eval().rbegin();
        }

        typename std::map<T, QuerySet<Args...>>::const_reverse_iterator rend() const {
            return this->eval().rend();
        }

    protected:
        // Eval (cache) queryset (make it protected?)
        const std::map<T, QuerySet<Args...>>& eval() const {
            SPDLOG_DEBUG(spdlog::get("qs"), "GroupedQuerySet[{}]::eval", (void*)this);
            if (!_evaluated) {
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
        QuerySet(const utils::ImplDataSource<Args...>& datasource) : BaseQs(datasource) {}

        QuerySet(const BaseQs& other) : BaseQs(other) {}

        const std::tuple<Args...> get(const typename std::tuple_element<0, std::tuple<Args...>>::type& t) {
            using T = typename std::tuple_element<0, std::tuple<Args...>>::type;
            auto r = utils::filter<Args...>(this->eval(), t);
            //auto r = this->filter<T>(t);
            if (r.size() > 1) {
                SPDLOG_DEBUG(spdlog::get("qs"), "QuerySet[{}]::get({}) -- multiple found ({} found)", (void*)this, t, r.size());
                throw qs::multiple_found<T>(t);
            }
            else if (r.size() == 0) {
                SPDLOG_DEBUG(spdlog::get("qs"), "QuerySet[{}]::get({}) -- not found", (void*)this, t);
                throw qs::not_found<T>(t);
            }
            else {
                return r[0];
            }
        }

        std::size_t count() const {
            return this->eval().size();
        }

        template <typename T>
        const std::vector<T> value_list() const {
            return utils::list<T>(this->eval());
        }

        const std::tuple<Args...>& operator[](const std::size_t& pos) const {
            return this->eval()[pos];
        }

        typename utils::queryset<Args...>::const_iterator begin() const {
            return this->eval().begin();
        }

        typename utils::queryset<Args...>::const_iterator end() const {
            return this->eval().end();
        }

        typename utils::queryset<Args...>::const_reverse_iterator rbegin() const {
            return this->eval().rbegin();
        }

        typename utils::queryset<Args...>::const_reverse_iterator rend() const {
            return this->eval().rend();
        }

        // Grouping by field types
        template <typename T>
        GroupedQuerySet<T, Args...> groupBy() {
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

