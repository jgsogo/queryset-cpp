
#pragma once

#include <type_traits>
#include <iostream>
#include <string>


namespace utils {

    // Use standard std::string if object does not have member function .to_string.
    template<class T>
    typename std::enable_if<std::is_same<decltype(std::declval<const T&>().to_string()), std::string>::value, std::string>::type to_string(const T &t) {
        return t.to_string();
    }

    template<class T>
    typename std::enable_if<std::is_same<decltype(std::to_string(std::declval<T&>())), std::string>::value, std::string>::type to_string(const T &t) {
        return std::to_string(t);
    }

    // TODO: This should be a specialization of the previous ones
    inline std::string to_string(const std::string& v) {
        return v;
    }
}