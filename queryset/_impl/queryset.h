
#pragma once

#include "base_queryset.h"
#include "../exceptions.h"

namespace qs {
    namespace _impl {

        template <typename Type, typename... Args>
        class QuerySet;

        template <typename Type, typename... Args>
        class OnMemoryQuerySet;

        template <typename T, typename Type, typename... Args>
        class GroupedQuerySet : public qs::_impl::BaseQuerySet<Type, Args...> {
            using BaseQs = qs::_impl::BaseQuerySet<Type, Args...>;
            public:
                GroupedQuerySet(const ImplDataSource<Type, Args...>& datasource, const bool lazy = false) : BaseQs(datasource), _evaluated(false), _lazy(lazy) {}
                GroupedQuerySet(const BaseQs& other, const bool lazy = false) : BaseQs(other), _evaluated(false), _lazy(lazy) {}
                GroupedQuerySet(const GroupedQuerySet<T, Type, Args...>& other, const bool lazy = false) : BaseQs(other),
                    _evaluated(false), _lazy(lazy) {}

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
                // Eval lazy queryset
                const std::map<T, QuerySet<Type, Args...>>& eval() const {
                    if (!_evaluated) {
                        SPDLOG_DEBUG(spdlog::get("qs"), "GroupedQuerySet[{}]::eval", (void*)this);
                        auto qs = this->BaseQs::eval();
                        auto values = utils::list<T, Args...>(qs.begin(), qs.end());
                        if (_lazy) {
                            for (auto& v : values) {
                                _qs.insert(std::make_pair(v, QuerySet<Type, Args...>(BaseQs::_datasource).filter(v)));
                            }
                        }
                        else {
                            std::map<T, OnMemoryQuerySet<Type, Args...>> data;
                            for (auto& v : values) {
                                data.insert(std::make_pair(v, OnMemoryQuerySet<Type, Args...>()));
                            }
                            constexpr std::size_t index = utils::tuple::index<T, Args...>();
                            for (auto& item : qs) {
                                const std::tuple<Args...>& tt = utils_queryset<Type, Args...>::as_tuple(item);
                                data[utils::getter<index>(tt)] >> item;
                            }
                            for (auto& d : data) {
                                _qs.insert(std::make_pair(d.first, static_cast<QuerySet<Type, Args...>>(d.second)));
                            }
                        }
                        _evaluated = true;
                    }
                    return _qs;
                }

            private:
                mutable std::map<T, QuerySet<Type, Args...>> _qs;
                mutable bool _evaluated;
                const bool _lazy;
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

					FilterContainer<Type, Args...> filter = _datasource.create_filter();
					filter.add_filter(t);
					auto r = filter.apply(this->eval());
                    
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
                    const queryset_type& qs_ = this->eval();
                    auto values = utils::list<T, Args...>(qs_.begin(), qs_.end());
                    return values;
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
                GroupedQuerySet<T, Type, Args...> groupBy(const bool lazy = true) {
                    // assert(!BaseQs::is_evaluated()); // TODO: Can I group-by after filtering?
                    return GroupedQuerySet<T, Type, Args...>(*this, lazy);
                }

                #ifdef WIP
                template <typename... Ts, typename = typename std::enable_if<(sizeof...(Ts) > 1), bool>::type>
                GroupedMultipleQuerySet<std::tuple<Ts...>, Args...> groupBy() {
                    return GroupedMultipleQuerySet<std::tuple<Ts...>, Args...>(*this);
                }
                #endif
        };


        template <typename Type, typename... Args>
        class OnMemoryQuerySet {
            using DataSource = MemoryQueryset<Type, Args...>;
            public:
                OnMemoryQuerySet() {};
                
                void operator >> (const typename std::conditional<std::is_same<Type, void>::value, std::tuple<Args...>, Type>::type& item) {
                    _memory_qs >> item;
                }

                operator QuerySet<Type, Args...>() const {
                    QuerySet<Type, Args...> ret(_memory_qs);
                    ret.count(); // TODO: Force evaluation, make it friend and hide this class
                    return ret;
                }

            protected:
                DataSource _memory_qs;
        };


    }
}
