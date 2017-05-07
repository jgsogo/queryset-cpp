
#pragma once

#include "../utils/queryset.h"


namespace qs {
    namespace _impl {

        template <typename Type, typename... Args>
        struct utils_queryset {
            typedef typename std::vector<Type> type;
			typedef typename std::tuple<Args...> tuple_type;
            static const std::tuple<Args...>& as_tuple(const Type& item) {
                return static_cast<std::tuple<Args...>&>(item);
            };
        };

		template <typename... Args>
		struct utils_queryset<std::tuple<Args...>, Args...> {
			typedef typename utils::queryset<Args...> type;
			typedef typename std::tuple<Args...> tuple_type;
			static const std::tuple<Args...>& as_tuple(const std::tuple<Args...>& item) {
				return item;
			};
		};

		/*
        template <typename... Args>
        struct utils_queryset<void, Args...> {
            typedef typename utils::queryset<Args...> type;
			typedef typename std::tuple<Args...> tuple_type;
            static const std::tuple<Args...>& as_tuple(const std::tuple<Args...>& item) {
                return item;
            };
        };
		*/

    }
}
