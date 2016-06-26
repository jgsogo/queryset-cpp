
#pragma once

#include "../utils/queryset.h"
#include "../datasource.h"
#include "../filters.h"
#include "utils.h"
//#include "../group_by.h"


namespace qs {
    namespace _impl {

        template <typename Type, typename... Args>
        class BaseQuerySet {
            public:
                typedef typename utils_queryset<Type, Args...>::type queryset_type;
            public:
                BaseQuerySet(const ImplDataSource<Type, Args...>& datasource) : _datasource(datasource), _evaluated(false) {};
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
                virtual void reset() { _evaluated = false; }

            protected:
                // Eval (cache) queryset (make it protected?)
                const queryset_type& eval() const {
                    SPDLOG_DEBUG(spdlog::get("qs"), "BaseQuerySet[{}]::eval", (void*)this);
                    if (!_evaluated) {
                        _qs = _datasource.apply(_filters);
                        _evaluated = true;
                    }
                    return _qs;
                }

            protected:
                FilterContainer<Type, Args...> _filters;

            private:
                const ImplDataSource<Type, Args...>& _datasource;
                mutable queryset_type _qs;
                mutable bool _evaluated;
        };

        template<typename R, typename Type, typename... Args>
        class BaseReturnQuerySet : public BaseQuerySet<Type, Args...> {
            protected:
                using BaseQs = BaseQuerySet<Type, Args...>;
            public:
                BaseReturnQuerySet(const ImplDataSource<Type, Args...>& datasource) : BaseQs(datasource) {};
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
}