
#pragma once

#include <memory>
#include "spdlog/spdlog.h"

#include "utils/queryset.h"
#include "filters.h"
#include "read_file.h"


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
            public:
                ImplDataSource() {}
                ImplDataSource(const ImplDataSource& other) : DataSource(other) {}
                virtual ~ImplDataSource() {}

                virtual qs_type apply(const FilterContainer<Type, Args...>& filters) const = 0;
        };

        template <typename Type, typename... Args>
        class MemoryQueryset : public ImplDataSource<Type, Args...> {
            public:
                using qs_type = typename ImplDataSource<Type, Args...>::qs_type;
            public:
                MemoryQueryset() : _qs(std::make_shared<qs_type>()) {}
                MemoryQueryset(const MemoryQueryset& other) : _qs(other._qs), ImplDataSource<Type, Args...>(other) {}
                virtual ~MemoryQueryset() {}

                void operator >>(const typename qs_type::value_type& item) {
                    _qs->push_back(item);
                }

                virtual qs_type apply(const FilterContainer<Type, Args...>& filters) const {
                    SPDLOG_DEBUG(spdlog::get("qs"), "DB HIT! -- MemoryQueryset::apply");
                    return filters.apply(*_qs);
                }

                std::size_t size() const {
                    return _qs->size();
                }
            protected:
                std::shared_ptr<qs_type> _qs;
        };

        template <typename Type, typename... Args>
        class FileQueryset : public ImplDataSource<Type, Args...> {
            public:
                using qs_type = typename ImplDataSource<Type, Args...>::qs_type;
            public:
                FileQueryset(const std::string& filename) : _filename(filename) {}
                FileQueryset(const FileQueryset& other) : _filename(other._filename), ImplDataSource<Type, Args...>(other) {}
                virtual ~FileQueryset() {}

                virtual qs_type apply(const FilterContainer<Type, Args...>& filters) const {
                    qs_type result;
                    SPDLOG_DEBUG(spdlog::get("qs"), "DB HIT! -- FileQueryset::apply");
                    utils::read_file<Type, Args...>(_filename, result);
                    return filters.apply(result);
                }

        protected:
                const std::string _filename;
        };
    }

    template <typename... Args>
    using MemoryQueryset = _impl::MemoryQueryset<void, Args...>;

    template <typename... Args>
    using FileQueryset = _impl::FileQueryset<void, Args...>;

    template <typename Type, typename... Args>
    using TypedMemoryQueryset = _impl::MemoryQueryset<Type, Args...>;

    template <typename Type, typename... Args>
    using TypedFileQueryset = _impl::FileQueryset<Type, Args...>;

}
