
#pragma once

#include "base_queryset.h"
#include "../exceptions.h"

namespace qs {
    namespace _impl {

        template <typename Type, typename... Args>
        class QuerySet;

        template <typename T, typename Type, typename... Args>
        class GroupedQuerySet : public qs::_impl::BaseQuerySet<Type, Args...> {
            using BaseQs = qs::_impl::BaseQuerySet<Type, Args...>;
            public:
                GroupedQuerySet(const ImplDataSource<Type, Args...>& datasource) : BaseQs(datasource), _evaluated(false) {}
                GroupedQuerySet(const BaseQs& other) : BaseQs(other), _evaluated(false) {}
                GroupedQuerySet(const GroupedQuerySet<T, Type, Args...>& other) : BaseQs(other),
                    _evaluated(false) {}

                std::size_t count() const {
                    return this->eval().size();
                }

                virtual bool is_evaluated() const {
                    return _evaluated;
                }

                virtual void reset() { _evaluated = false; }

                const QuerySet<Type, Args...>& at(const T& t) const {
                    return this->eval().at(t);
                }

                typename std::map<T, QuerySet<Type, Args...>>::const_iterator begin() const {
                    return this->eval().begin();
                }

                typename std::map<T, QuerySet<Type, Args...>>::const_iterator end() const {
                    return this->eval().end();
                }

                typename std::map<T, QuerySet<Type, Args...>>::const_reverse_iterator rbegin() const {
                    return this->eval().rbegin();
                }

                typename std::map<T, QuerySet<Type, Args...>>::const_reverse_iterator rend() const {
                    return this->eval().rend();
                }

            protected:
                // Eval (cache) queryset (make it protected?)
                const std::map<T, QuerySet<Type, Args...>>& eval() const {
                    SPDLOG_DEBUG(spdlog::get("qs"), "GroupedQuerySet[{}]::eval", (void*)this);
                    if (!_evaluated) {
                        auto qs = this->BaseQs::eval();
                        auto values = utils::list<T, Args...>(qs.begin(), qs.end());
                        for (auto& v : values) {
                            _qs.insert(std::make_pair(v, QuerySet<Type, Args...>(*this).filter(v)));
                        }
                        _evaluated = true;
                    }
                    return _qs;
                }

            private:
                mutable std::map<T, QuerySet<Type, Args...>> _qs;
                mutable bool _evaluated;
        };

        #ifdef WIP
        template <typename T, typename... Args>
        class GroupedMultipleQuerySet : public qs::_impl::BaseReturnQuerySet<GroupedMultipleQuerySet<T, Args...>, Args...> {
            protected:
                using BaseQs = qs::_impl::BaseReturnQuerySet<GroupedMultipleQuerySet<T, Args...>, Args...>;
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


        template <typename Type, typename... Args>
        class QuerySet : public qs::_impl::BaseReturnQuerySet<QuerySet<Type, Args...>, Type, Args...> {
            using BaseQs = qs::_impl::BaseReturnQuerySet<QuerySet<Type, Args...>, Type, Args...>;
            using queryset_type = typename BaseQs::queryset_type;
            using value_type = typename BaseQs::queryset_type::value_type;
            public:
                QuerySet(const ImplDataSource<Type, Args...>& datasource) : BaseQs(datasource) {}

                QuerySet(const BaseQs& other) : BaseQs(other) {}

                const value_type get(const typename std::tuple_element<0, std::tuple<Args...>>::type& t) {
                    using T = typename std::tuple_element<0, std::tuple<Args...>>::type;
                    const queryset_type& qs_ = this->eval();
                    auto it = std::find(qs_.begin(), qs_.end(), t);
                    return (*it);
                    /*
                    utils::filter<Args..., T, queryset_type>(this->eval(), t, r);
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
                    */
                }

                std::size_t count() const {
                    return this->eval().size();
                }

                template <typename T>
                const std::vector<T> value_list() const {
                    return utils::list<T>(this->eval());
                }

                const value_type& operator[](const std::size_t& pos) const {
                    return this->eval()[pos];
                }

                typename queryset_type::const_iterator begin() const {
                    return this->eval().begin();
                }

                typename queryset_type::const_iterator end() const {
                    return this->eval().end();
                }

                typename queryset_type::const_reverse_iterator rbegin() const {
                    return this->eval().rbegin();
                }

                typename queryset_type::const_reverse_iterator rend() const {
                    return this->eval().rend();
                }

                // Grouping by field types
                template <typename T>
                GroupedQuerySet<T, Type, Args...> groupBy() {
                    assert(!BaseQs::is_evaluated());
                    return GroupedQuerySet<T, Type, Args...>(*this);
                }

                #ifdef WIP
                template <typename... Ts, typename = typename std::enable_if<(sizeof...(Ts) > 1), bool>::type>
                GroupedMultipleQuerySet<std::tuple<Ts...>, Args...> groupBy() {
                    return GroupedMultipleQuerySet<std::tuple<Ts...>, Args...>(*this);
                }
                #endif
        };

    }
}