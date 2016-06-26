
#pragma once

#include "../utils/queryset.h"


namespace qs {
    namespace _impl {

        template <typename Type, typename... Args>
        struct utils_queryset {
            typedef typename std::vector<Type> type;
        };

        template <typename... Args>
        struct utils_queryset<void, Args...> {
            typedef typename utils::queryset<Args...> type;
        };

    }
}