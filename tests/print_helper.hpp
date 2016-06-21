
#include "../queryset/utils/tuple.h"

namespace boost {
    namespace test_tools {
        namespace tt_detail {

            template <typename... Args>
            struct print_log_value<std::tuple<Args...> >
            {
                void operator()(std::ostream& os, const std::tuple<Args...>& pr)
                {
                    // TODO: Why just `os << pr` or `::operator<<(os, pr)` does not work?
                    os << "(";
                    ::utils::tuple::print_tuple_impl(os, pr, std::index_sequence_for<Args...>{});
                    os << ")";
                }
            };

        }
    }
}

