
#pragma once

#include <exception>
#include <ostream>

namespace qs {
    
    template <typename T>
    class not_found : public std::runtime_error {
        public:
            not_found(const T& t) : std::runtime_error("not_found exception"), _pk(t) {}
            const char* what() const throw() {
                std::ostringstream os;
                os << "Item pk='" << _pk << "' not found.";
                return os.str().c_str();
            }
        protected:
            T _pk;
    };

    template <typename T>
    class multiple_found : public std::runtime_error {
        public:
            multiple_found(const T& t) : std::runtime_error("multiple_found exception"), _pk(t) {}
            const char* what() const throw() {
                std::ostringstream os;
                os << "Multiple objects found with pk='" << _pk << "' not found.";
                return os.str().c_str();
            }
        protected:
            T _pk;
    };
}
