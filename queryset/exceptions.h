
#pragma once

#include <exception>
#include <ostream>

namespace qs {
    typedef std::exception exception;
    
    template <typename T>
    class not_found : public exception {
        public:
            not_found(const T& t) : exception("not_found exception"), _pk(t) {}
            const char* what() const throw() {
                std::ostringstream os;
                os << "Item pk='" << _pk << "' not found.";
                return os.str().c_str();
            }
        protected:
            T _pk;
    };

    template <typename T>
    class multiple_found : public exception {
        public:
            multiple_found(const T& t) : exception("multiple_found exception"), _pk(t) {}
            const char* what() const throw() {
                std::ostringstream os;
                os << "Multiple objects found with pk='" << _pk << "' not found.";
                return os.str().c_str();
            }
        protected:
            T _pk;
    };
}