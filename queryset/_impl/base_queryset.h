
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
                BaseQuerySet(const ImplDataSource<Type, Args...>& datasource) : _datasource(datasource), _filters(datasource.create_filter()) {};
                BaseQuerySet(const BaseQuerySet& other) : _datasource(other._datasource),
                    _filters(other._filters),
                    _qs(other._qs) {}

                bool empty() const {
                    if (!_qs) {
                        this->eval();
                    }
                    return _qs->empty();
                }

                virtual bool is_evaluated() const {
                    return (bool)_qs;
                }
                virtual void reset() { _qs.reset(); }

                void remove() {
                    _datasource.remove(_filters);
                    this->reset(); // TODO: ¿Quiero invalidar la queryset para que no se vuelva a utilizar?
                }

            protected:
                // Eval (cache) queryset (make it protected?)
                const queryset_type& eval() const {
                    if (!_qs) {
                        SPDLOG_DEBUG(spdlog::get("qs"), "BaseQuerySet[{}]::eval", (void*)this);
                        _qs = std::make_shared<queryset_type>(_datasource.apply(_filters));
                    }
                    return *_qs.get();
                }

            protected:
                FilterContainer<Type, Args...> _filters;
                const ImplDataSource<Type, Args...>& _datasource;
            private:
                mutable std::shared_ptr<queryset_type> _qs;
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
                R& filter(const std::vector<T> &filter_value) {
                    assert(!BaseQs::is_evaluated());
                    BaseQs::_filters.template add_filter<T>(filter_value);
                    return static_cast<R&>(*this);
                }

                template<typename T>
                R& filter(const T &filter_value) {
                    assert(!BaseQs::is_evaluated());
                    BaseQs::_filters.template add_filter<T>(filter_value);
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
