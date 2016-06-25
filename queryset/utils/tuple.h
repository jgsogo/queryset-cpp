
#pragma once

#include <tuple>
#include <utility>
#include <ostream>

namespace utils {
    namespace tuple {
        
        // References: 
        //  - a hpp with some interesting functionality: https://github.com/meox/omega.hpp
        //  - tail/head: http://stackoverflow.com/questions/10626856/how-to-split-a-tuple
        //  - project: http://stackoverflow.com/questions/23612648/creating-a-c-stdtuple-projection-function
        //  - index of type: http://stackoverflow.com/questions/18063451/get-index-of-a-tuple-elements-type
        //  - remove ith type of tuple: http://stackoverflow.com/questions/14852593/removing-the-first-type-of-a-stdtuple
        //  - integer sequence with one out: http://stackoverflow.com/questions/27124920/compile-time-generate-integer-sequence-with-one-left-out
        //  - iterate a tuple: http://stackoverflow.com/questions/1198260/iterate-over-tuple
        //  - reverse tuple: https://stackoverflow.com/questions/17178075/how-do-i-reverse-a-tuple/17178399#17178399
        
        // Projection
        namespace {
            template <typename T, size_t... indexes>
            class Projection {
                public:
                    using Tuple = std::tuple<typename std::tuple_element<indexes, T>::type...>;
            };

            template <std::size_t... Ns, typename... Ts >
            auto tail_impl(std::index_sequence<Ns...>, std::tuple<Ts...> t) {
                return std::make_tuple(std::get<Ns + 1u>(t)...);
            }
        }

        template <typename T , typename... Ts>
        auto head(std::tuple<T,Ts...> t ) {
           return std::get<0>(t);
        }

        template <typename... Ts>
        auto tail(std::tuple<Ts...> t ) {
           return tail_impl(std::make_index_sequence<sizeof...(Ts) - 1u>() , t );
        }

        template <std::size_t... indexes, typename T>
        auto project(const T &t) -> typename Projection<T, indexes...>::Tuple {
            return typename Projection<T, indexes...>::Tuple(std::get<indexes>(t)...);
        }

        template <std::size_t... indexes, typename T>
        auto project(const T &t, std::index_sequence<indexes...>) {
            return std::make_tuple(std::get<indexes>(t)...);
        }

        // Index of a given type
        namespace {
            template <class T, class Tuple>
            struct Index;

            template <class T, class... Types>
            struct Index<T, std::tuple<T, Types...>> {
                static const std::size_t value = 0;
            };

            template <class T, class U, class... Types>
            struct Index<T, std::tuple<U, Types...>> {
                static const std::size_t value = 1 + Index<T, std::tuple<Types...>>::value;
            };
        }
        
        template <typename T, typename... Ts>
        constexpr std::size_t index() {
            return Index<T, std::tuple<Ts...>>::value;
        }

        // Remove ith type of a tuple
        template<size_t I, typename T>
        struct remove_ith_type;

        template<typename T, typename... Ts>
        struct remove_ith_type<0, std::tuple<T, Ts...>> {
            typedef std::tuple<Ts...> type;
        };

        template<size_t I, typename T, typename... Ts>
        struct remove_ith_type<I, std::tuple<T, Ts...>> {
            typedef decltype(
                std::tuple_cat(
                    std::declval<std::tuple<T>>(),
                    std::declval<typename remove_ith_type<I - 1, std::tuple<Ts...>>::type>()
                )
                ) type;
        };

        // Generate integer sequence with one out:
        namespace {
            template <typename Seq1, std::size_t Offset, typename Seq2> struct concat_seq;

            template <std::size_t ... Is1, std::size_t Offset, std::size_t ... Is2>
            struct concat_seq<std::index_sequence<Is1...>, Offset, std::index_sequence<Is2...>>
            {
                using type = std::index_sequence<Is1..., (Offset + Is2)...>;
            };
        }

        template <std::size_t N, std::size_t E>
        using gen_seq = typename concat_seq<typename std::make_index_sequence<E>, E + 1, typename std::make_index_sequence<(N > E) ? (N - E - 1) : 0>>::type;


        // Comparaison
        //struct NoCompareType {};
        namespace {
            template <typename T, typename U = T>
            struct atomic_compare {
                static bool pair_compare(const T& lhs, const U& rhs) {
                    return (lhs == rhs);
                }
            };
            /*
            template <typename T>
            struct atomic_compare<T, NoCompareType> {
                static bool pair_compare(const T& lhs, const NoCompareType& rhs) {
                    std::cout << "pair_compare(lhs, NoCompareType)" << std::endl;
                    return true;
                }
            };
            
            template <typename T>
            struct atomic_compare<NoCompareType, T> {
                static bool pair_compare(const NoCompareType& lhs, const T& rhs) {
                    std::cout << "pair_compare(NoCompareType, rhs)" << std::endl;
                    return true;
                }
            };
            */
        }
        
        template <typename T>
        bool pair_compare(const std::tuple<T>& lhs, const std::tuple<T>& rhs) {
            return atomic_compare<T>::pair_compare(head(lhs), head(rhs));
        }

        template <typename T, typename... Args, typename = typename std::enable_if<(sizeof...(Args) > 0), bool>::type>
        bool pair_compare(const std::tuple<T, Args...>& lhs, const std::tuple<T, Args...>& rhs) {
            return atomic_compare<T>::pair_compare(head(lhs), head(rhs)) && 
                   pair_compare<Args...>(tail(lhs), tail(rhs));
        }
        
        // Compare tuples of different size.
        template <typename T, typename... Us>
        auto pair_compare(const std::tuple<T>& lhs, const std::tuple<Us...>& rhs) {
            constexpr std::size_t index = ::utils::tuple::index<T, Us...>();
            return atomic_compare<T>::pair_compare(head(lhs), std::get<index>(rhs));
        }

        template <typename... Ts, typename... Us>
        auto pair_compare(const std::tuple<Ts...>& lhs, const std::tuple<Us...>& rhs)
            -> typename std::enable_if<(sizeof...(Ts) <= sizeof...(Us)), bool>::type
        {
            typedef typename std::tuple_element<0, std::tuple<Ts...> >::type lhs_head_type;
            constexpr std::size_t index = ::utils::tuple::index<lhs_head_type, Us...>();
            return atomic_compare<lhs_head_type>::pair_compare(head(lhs), std::get<index>(rhs)) &&
                pair_compare(tail(lhs), rhs);
        }

        template <typename... Ts, typename... Us>
        auto pair_compare(const std::tuple<Ts...>& lhs, const std::tuple<Us...>& rhs)
            -> typename std::enable_if<(sizeof...(Ts) > sizeof...(Us)), bool>::type
        {
            return pair_compare(rhs, lhs);
        }

        // Print
        // pretty-print a tuple (from http://stackoverflow.com/a/6245777/273767)
        template<class Ch, class Tr, class Tuple, std::size_t... Is>
        void print_tuple_impl(std::basic_ostream<Ch, Tr>& os,
                              const Tuple & t,
                              std::index_sequence<Is...>) {
            using ::operator<<;
            using swallow = int[]; // guaranties left to right order
            (void)swallow {
                0, (void(os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), 0)...
            };
        }

        // Iterate over a tuple
        template<std::size_t I = 0, typename FuncT, typename... Tp>
        inline typename std::enable_if<I == sizeof...(Tp), void>::type
        for_each(const std::tuple<Tp...> &, FuncT) { }

        template<std::size_t I = 0, typename FuncT, typename... Tp>
        inline typename std::enable_if<I < sizeof...(Tp), void>::type
        for_each(const std::tuple<Tp...>& t, FuncT f) {
            f(std::get<I>(t));
            for_each<I + 1, FuncT, Tp...>(t, f);
        }

        /*
        // Reverse a tuple
        template <typename... Ts>
        struct tuple_reverse;

        template <>
        struct tuple_reverse<std::tuple<>>
        {
            using type = std::tuple<>;
        };

        template <typename T, typename... Ts>
        struct tuple_reverse<std::tuple<T, Ts...>>
        {
            using head = std::tuple<T>;
            using tail = typename tuple_reverse<std::tuple<Ts...>>::type;

            using type = decltype(std::tuple_cat(std::declval<tail>(), std::declval<head>()));
        };

        template<typename T, typename TT = typename std::remove_reference<T>::type, size_t... I>
        auto
            reverse_impl(T&& t, std::index_sequence<I...>)
            -> std::tuple<typename std::tuple_element<sizeof...(I)-1 - I, TT>::type...>
        {
            return std::make_tuple(std::get<sizeof...(I)-1 - I>(std::forward<T>(t))...);
        }

        template<typename T, typename TT = typename std::remove_reference<T>::type>
        auto
            reverse(T&& t)
            -> decltype(reverse_impl(std::forward<T>(t),
                redi::make_index_sequence<std::tuple_size<TT>::value>()))
        {
            return reverse_impl(std::forward<T>(t),
                redi::make_index_sequence<std::tuple_size<TT>::value>());
        }
        */
    }
}

template<class Ch, class Tr, class... Args>
decltype(auto) operator<<(std::basic_ostream<Ch, Tr>& os,
    const std::tuple<Args...>& t)
{
    os << "(";
    ::utils::tuple::print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
    return os << ")";
}
