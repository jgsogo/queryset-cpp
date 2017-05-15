
#pragma once

#include <type_traits>
#include <iostream>
#include <string>
#include "../strong_typedef.h"


namespace utils {

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

	namespace {
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
    inline std::string to_string(const T& t) {
        return to_string_impl(t, std::integral_constant<bool, HasToStringMethod<T>::Has>());
    }

    template <>
    inline std::string to_string<std::string>(const std::string& t) {
        return t;
    }
	
	template <class T, int N>
	inline std::string to_string(const strong_typedef<T, N>& t) {
		return to_string(t.value);
	}

	template <class T,  typename... Args>
	void call_to_string(const T& t, Args... args, std::function<void (const std::string&, Args...)> f) {
		f(to_string(t), args...);
	}

}