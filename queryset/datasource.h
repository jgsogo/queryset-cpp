
#pragma once

#include <memory>
#include "easylogging++.h"
#include "utils/queryset.h"
#include "filters.h"
#include "read_file.h"


namespace utils {

    class DataSource {
        public:
            DataSource() {}
            DataSource(const DataSource& other) {}
            virtual ~DataSource() {}
    };

    template <typename... Args>
    class ImplDataSource : public DataSource {
        public:
            using qs_type = queryset<Args...>;
        public:
            ImplDataSource() {}
            ImplDataSource(const ImplDataSource& other) : DataSource(other) {}
            virtual ~ImplDataSource() {}

            virtual qs_type apply(const utils::FilterContainer<Args...>& filters) const = 0;
    };

    template <typename... Args>
    class MemoryQueryset : public ImplDataSource<Args...> {
        public:
            using qs_type = typename ImplDataSource<Args...>::qs_type;
        public:
            MemoryQueryset() : _qs(std::make_shared<qs_type>()) {
                LOG(DEBUG) << "MemoryQueryset<Args...>::MemoryQueryset()";
            }
            MemoryQueryset(const MemoryQueryset& other) : _qs(other._qs), ImplDataSource<Args...>(other) {
                LOG(DEBUG) << "MemoryQueryset<Args...>::MemoryQueryset(const MemoryQueryset& other)";
            }
            virtual ~MemoryQueryset() {}

            void operator >>(const typename qs_type::value_type& item) {
                _qs->push_back(item);
            }

            virtual qs_type apply(const utils::FilterContainer<Args...>& filters) const {
                LOG(DEBUG) << "MemoryQueryset<Args...>::apply(filters)";
                return filters.apply(*_qs);
            }

            std::size_t size() const {
                return _qs->size();
            }
        protected:
            std::shared_ptr<qs_type> _qs;
    };

    template <typename... Args>
    class FileQueryset : public ImplDataSource<Args...> {
        public:
            using qs_type = typename ImplDataSource<Args...>::qs_type;
        public:
            FileQueryset(const std::string& filename) : _filename(filename) {
                LOG(DEBUG) << "FileQueryset<Args...>::FileQueryset(filename='" << filename << "')";
            }
            FileQueryset(const FileQueryset& other) : _filename(other._filename), ImplDataSource<Args...>(other) {
                LOG(DEBUG) << "FileQueryset<Args...>::FileQueryset(const FileQueryset& other)";
            }
            virtual ~FileQueryset() {}

            virtual qs_type apply(const utils::FilterContainer<Args...>& filters) const {
                LOG(DEBUG) << "FileQueryset<Args...>::apply(filters)";
                queryset<Args...> result;
                read_file(_filename, result);
                return filters.apply(result);
            }

    protected:
            const std::string _filename;
    };
}
