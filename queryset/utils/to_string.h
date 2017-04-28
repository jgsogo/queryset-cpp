
#pragma once

#include <type_traits>
#include <iostream>
#include <string>


namespace utils {

    namespace {
        template<class T>
        struct HasToStringMethod
        {
            typedef char(&Yes)[1];
            typedef char(&No)[2];

            template<class U>
            static Yes test(U* data,
                typename std::enable_if<std::is_same<
                decltype(data->to_string()), std::string
                >::value>::type * = 0);
            static No test(...);
            static const bool Has = sizeof(Yes) == sizeof(HasToStringMethod::test((typename std::remove_reference<T>::type*)0));
        };

        template <class T>
        std::string to_string_impl(const T& t, std::false_type) {
            return std::to_string(t);
        }

        template <class T>
        std::string to_string_impl(const T& t, std::true_type) {
            return t.to_string();
        }
    }

    template <class T>
    std::string to_string(const T& t) {
        return to_string_impl(t, std::integral_constant<bool, HasToStringMethod<T>::Has>());
    }

    template <>
    inline std::string to_string<std::string>(const std::string& t) {
        return t;
    }

    /*

    // Use standard std::string if object does not have member function .to_string.
    template<class T, typename = typename std::enable_if<std::is_same<decltype(std::declval<const T&>().to_string()), std::string>::value, std::string>::type>
    auto to_string(const T& t, typename std::enable_if<std::is_same<decltype(std::declval<const T&>().to_string()), std::string>::value, std::string>::type* dummy = nullptr) {
        return t.to_string();
    }

    template<class T, typename = typename std::enable_if<std::is_same<decltype(std::to_string(std::declval<T&>())), std::string>::value, std::string>::type>
    auto to_string(const T& t, typename std::enable_if<std::is_same<decltype(std::to_string(std::declval<T&>())), std::string>::value, std::string>::type* dummy = nullptr) {
        return std::to_string(t);
    }
    */
    /*
    template<class T>
    typename std::enable_if<std::is_same<decltype(std::declval<const T&>().to_string()), std::string>::value, std::string>::type to_string(const T &t) {
        return t.to_string();
    }

    template<class T>
    typename std::enable_if<std::is_same<decltype(std::to_string(std::declval<T&>())), std::string>::value, std::string>::type to_string(const T &t) {
        return std::to_string(t);
    }
    */
    /*
    // TODO: This should be a specialization of the previous ones
    inline std::string to_string(const std::string& v) {
        return v;
    }
    */
}