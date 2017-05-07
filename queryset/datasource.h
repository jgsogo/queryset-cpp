
#pragma once

#include <memory>
#include "spdlog/spdlog.h"

#include "utils/queryset.h"
#include "filters.h"

namespace qs {

    namespace _impl {

        class DataSource {
            public:
                DataSource() {}
                DataSource(const DataSource& other) {}
                virtual ~DataSource() {}
        };

        template <typename Type, typename... Args>
        class ImplDataSource : public DataSource {
            public:
                using qs_type = typename qs::_impl::utils_queryset<Type, Args...>::type;
				using tuple_type = typename qs::_impl::utils_queryset<Type, Args...>::tuple_type;
            public:
                ImplDataSource() {}
                ImplDataSource(const ImplDataSource& other) : DataSource(other) {}
                virtual ~ImplDataSource() {}

				virtual _impl::FilterContainer<Type, Args...> create_filter() const {
					return _impl::FilterContainer<Type, Args...>();
				}

                virtual qs_type apply(const FilterContainer<Type, Args...>& filters) const = 0;
                virtual void remove(const FilterContainer<Type, Args...>& filters) const {
                    throw std::runtime_error("Method 'delete' not implemented");
                };

				virtual void insert(const std::tuple<Args...>& item) {
					throw std::runtime_error("Method 'insert' not implemented");
				}
        };

        template <typename Type, typename... Args>
        class MemoryDataSource : public ImplDataSource<Type, Args...> {
            public:
                using qs_type = typename ImplDataSource<Type, Args...>::qs_type;
            public:
                MemoryDataSource() : _qs(std::make_shared<qs_type>()) {}
                MemoryDataSource(const MemoryDataSource& other) : _qs(other._qs), ImplDataSource<Type, Args...>(other) {}
                virtual ~MemoryDataSource() {}

                void operator >>(const typename qs_type::value_type& item) {
                    _qs->push_back(item);
                }

				virtual FilterContainer<Type, Args...> create_filter() const {
					return FilterContainer<Type, Args...>();
				}

                virtual qs_type apply(const FilterContainer<Type, Args...>& filters) const {
                    SPDLOG_DEBUG(spdlog::get("qs"), "DB HIT! -- FileContainer::apply");
                    return filters.apply(*_qs);
                }

                std::size_t size() const {
                    return _qs->size();
                }
            protected:
                std::shared_ptr<qs_type> _qs;
        };

        
    }

    //template <typename... Args>
    //using MemoryDataSource = _impl::MemoryDataSource<void, Args...>;

    template <typename Type, typename... Args>
    using MemoryDataSource = _impl::MemoryDataSource<Type, Args...>;
}
