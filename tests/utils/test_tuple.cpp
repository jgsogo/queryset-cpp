
#include <boost/test/unit_test.hpp>

#include "print_helper.hpp"
#include "../../queryset/utils/tuple.h"

BOOST_AUTO_TEST_SUITE(tuple)

typedef std::tuple<int, std::string, float> mytuple;

BOOST_AUTO_TEST_CASE(tuple_index)
{
    BOOST_CHECK_EQUAL((::utils::tuple::index<int, int, std::string, float>()), 0);
    BOOST_CHECK_EQUAL((::utils::tuple::index<std::string, int, std::string, float>()), 1);
    BOOST_CHECK_EQUAL((::utils::tuple::index<float, int, std::string, float>()), 2);
}

BOOST_AUTO_TEST_CASE(tuple_projection)
{
    mytuple a{2, "hola", 0.f};

    // Indexes for each element
    constexpr std::size_t index_int = ::utils::tuple::index<int, int, std::string, float>();
    constexpr std::size_t index_string = ::utils::tuple::index<std::string, int, std::string, float>();
    constexpr std::size_t index_float = ::utils::tuple::index<float, int, std::string, float>();

    // Basic functions: 'head' and 'tail'
    BOOST_CHECK_EQUAL(::utils::tuple::head(a), 2);
    BOOST_CHECK_EQUAL(::utils::tuple::tail(a), (std::tuple<std::string, float>("hola", 0.f)));

    // Projection on one axis
    BOOST_CHECK_EQUAL(::utils::tuple::project<index_int>(a), std::tuple<int>(2));
    BOOST_CHECK_EQUAL(::utils::tuple::project<index_string>(a), std::tuple<std::string>("hola"));
    BOOST_CHECK_EQUAL(::utils::tuple::project<index_float>(a), std::tuple<float>(0.f));

    BOOST_CHECK_EQUAL(::utils::tuple::project<0>(a), std::tuple<int>(2));
    BOOST_CHECK_EQUAL(::utils::tuple::project<1>(a), std::tuple<std::string>("hola"));
    BOOST_CHECK_EQUAL(::utils::tuple::project<2>(a), std::tuple<float>(0.f));

    // Projection on several axis
    BOOST_CHECK_EQUAL((::utils::tuple::project<0, 1>(a)), (std::tuple<int, std::string>(2, "hola")));
    BOOST_CHECK_EQUAL((::utils::tuple::project<1, 2>(a)), (std::tuple<std::string, float>("hola", 0.f)));
    BOOST_CHECK_EQUAL((::utils::tuple::project<0, 2>(a)), (std::tuple<int, float>(2, 0.f)));

    // Unordered projection
    BOOST_CHECK_EQUAL((::utils::tuple::project<2, 0>(a)), (std::tuple<float, int>(0.f, 2)));
}

BOOST_AUTO_TEST_CASE(tuple_remove_ith_type)
{
    typedef ::utils::tuple::remove_ith_type<0, mytuple>::type mytuple_12;
    typedef ::utils::tuple::remove_ith_type<1, mytuple>::type mytuple_02;
    typedef ::utils::tuple::remove_ith_type<2, mytuple>::type mytuple_01;

    BOOST_CHECK_EQUAL((mytuple_12{ "a", 0.f }), (std::tuple<std::string, float>{"a", 0.f}));
    BOOST_CHECK_EQUAL((mytuple_02{ 0, 0.f }), (std::tuple<int, float>{0, 0.f}));
    BOOST_CHECK_EQUAL((mytuple_01{ 0, "a" }), (std::tuple<int, std::string>{0, "a"}));
}

BOOST_AUTO_TEST_CASE(tuple_gen_seq)
{
    mytuple a{ 2, "hola", 0.f };

    typedef ::utils::tuple::gen_seq<3, 0> seq1;
    typedef ::utils::tuple::gen_seq<3, 1> seq2;
    typedef ::utils::tuple::gen_seq<3, 2> seq3;

    // It's used to project
    BOOST_CHECK_EQUAL(::utils::tuple::project(a, seq1()), (std::tuple<std::string, float>("hola", 0.f)));
    BOOST_CHECK_EQUAL(::utils::tuple::project(a, seq2()), (std::tuple<int, float>(2, 0.f)));
    BOOST_CHECK_EQUAL(::utils::tuple::project(a, seq3()), (std::tuple<int, std::string>(2, "hola")));
    
    // This should give a compiler error
    typedef ::utils::tuple::gen_seq<3, 3> seq4;
    BOOST_CHECK_EQUAL(::utils::tuple::project(a, seq4()), a);
}

BOOST_AUTO_TEST_CASE(tuple_comparaison_same_size)
{
    mytuple a{ 2, "hola", 0.f };
    mytuple b1{ 3, "hola", 0.f };
    mytuple b2{ 2, "bye", 0.f };
    mytuple b3{ 2, "hola", 1.f };

    BOOST_CHECK(utils::tuple::pair_compare(a, a));
    BOOST_CHECK(!utils::tuple::pair_compare(a, b1));
    BOOST_CHECK(!utils::tuple::pair_compare(a, b2));
    BOOST_CHECK(!utils::tuple::pair_compare(a, b3));
    BOOST_CHECK(!utils::tuple::pair_compare(b1, a));
    BOOST_CHECK(!utils::tuple::pair_compare(b2, a));
    BOOST_CHECK(!utils::tuple::pair_compare(b3, a));
}

BOOST_AUTO_TEST_CASE(tuple_comparaison_different_size)
{
    mytuple a{ 2, "hola", 0.f };

    // Size == 1
    auto b1 = std::make_tuple<int>(2);
    auto b2 = std::make_tuple<std::string>("hola");
    auto b3 = std::make_tuple<float>(0.f);

    BOOST_CHECK(utils::tuple::pair_compare(a, b1));
    BOOST_CHECK(utils::tuple::pair_compare(a, b2));
    BOOST_CHECK(utils::tuple::pair_compare(a, b3));

    BOOST_CHECK(utils::tuple::pair_compare(b1, a));
    BOOST_CHECK(utils::tuple::pair_compare(b2, a));
    BOOST_CHECK(utils::tuple::pair_compare(b3, a));

    // Size == 2
    auto c1 = std::make_tuple<int, float>(2, 0.f);
    BOOST_CHECK(utils::tuple::pair_compare(a, c1));
    BOOST_CHECK(utils::tuple::pair_compare(c1, a));

    auto d1 = std::make_tuple<float, int>(0.f, 2);
    BOOST_CHECK(utils::tuple::pair_compare(a, d1));
    BOOST_CHECK(utils::tuple::pair_compare(d1, a));
}

BOOST_AUTO_TEST_CASE(tuple_gt)
{
    mytuple a{ 2, "hola", 0.f };
    BOOST_CHECK_GT(a, (mytuple{ 1, "z", 23.f }));
    BOOST_CHECK_GT(a, (mytuple{ 2, "a", 23.f }));
    BOOST_CHECK_GT(a, (mytuple{ 2, "hola", -1.f }));
}

BOOST_AUTO_TEST_SUITE_END()
