
#pragma once

#include "read_file.h"
#include "utils/tuple.h"
#include "utils/queryset.h"

namespace utils {
        
    typedef std::runtime_error Exception;
        
    class NotFoundException : public Exception {
        public:
            NotFoundException(const char* msg) : Exception(msg) {};
    };
               
    template <typename... Args>
    class BaseManager {
        public:
            typedef queryset<Args...> _t_queryset;
                
            static std::size_t parse(const std::string& filename, _t_queryset& data) {
                std::size_t old_length = data.size();
                read_file<Args...>(filename, data);
                return data.size() - old_length;
            }
                
        public:
            BaseManager(const std::string& filename) {
                parse(filename, _raw_data);
            }

            template <typename T>
            auto filter(const T& filter_value) {
                return ::utils::filter(_raw_data, filter_value);
            }

            template <typename T>
            auto filter(const std::vector<T>& filter_values) {
                return ::utils::filter(_raw_data, filter_values);
            }

            const _t_queryset& all() const {
                return _raw_data;
            }
        protected:
            _t_queryset _raw_data;
    };     
}

