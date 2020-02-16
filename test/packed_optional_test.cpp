#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "packed_optional/packed_optional.hpp"

namespace po = packed_optional;
constexpr int g_empty_value{ -1 };
constexpr int g_other_empty_value{ -3 };
constexpr char g_empty_value_c{ 1 };
constexpr int g_default_value{ 42 };
constexpr char g_default_value_c{ 42 };
constexpr int g_value{ 10 };
using popt = po::packed_optional<int, g_empty_value>;
using popt_other = po::packed_optional<int, g_other_empty_value>;
using popt_c = po::packed_optional<char, g_empty_value_c>;

template<typename T, T empty_value>
void require_empty(po::packed_optional<T, empty_value> opt) {
    REQUIRE_FALSE(opt.has_value());
    REQUIRE_FALSE(opt);

    REQUIRE_THROWS_AS(opt.value(), po::bad_optional_access);
    REQUIRE(opt.value_or(g_default_value) == g_default_value);
}

template<typename T, T empty_value>
void require_has_value(po::packed_optional<T, empty_value> opt, T value) {
    REQUIRE(opt.has_value());
    REQUIRE(opt);

    REQUIRE(opt.value() == value);
    REQUIRE(opt.value_or(g_default_value) == value);
    REQUIRE(*opt == value);
}

template<typename T1, typename T2, T1 empty_value1, T2 empty_value2>
void require_equal(const po::packed_optional<T1, empty_value1>& p1, 
                   const po::packed_optional<T2, empty_value2>& p2){
    REQUIRE(p1 == p2);
    REQUIRE_FALSE(p1 != p2);
}

template<typename T1, typename T2, T1 empty_value1, T2 empty_value2>
void require_not_equal(const po::packed_optional<T1, empty_value1>& p1, 
                   const po::packed_optional<T2, empty_value2>& p2){
    REQUIRE(p1 != p2);
    REQUIRE_FALSE(p1 == p2);
}

TEST_CASE("Creation") {
    SUBCASE("Empty") {
        popt opt;

        require_empty(opt);
    }

    SUBCASE("From empty value") {
        popt opt{ g_empty_value };

        require_empty(opt);
    }

    SUBCASE("From value") {
        popt opt{ g_value };

        require_has_value(opt, g_value);
    }

    SUBCASE("From other packed_optional") {
        SUBCASE("Empty") {
            popt opt;
            popt copy{ opt };

            require_empty(copy);
        }

        SUBCASE("With value") {
            popt opt{ g_value };
            popt copy{ opt };

            require_has_value(copy, g_value);
        }
    }
}

TEST_CASE("Equality") {
    {
        popt o1;

        {
            popt o2;

            require_equal(o1, o2);
        }

        {
            popt o2{g_empty_value};
            
            require_equal(o1, o2);
        }
    }

    {
        popt o1{g_value};
        popt o2{g_value};
        
        require_equal(o1, o2);
    }

    {
        popt o1{g_value};
        popt o2{o1};
        
        require_equal(o1, o2);
    }

    {
        popt o1;
        popt_other o2;
        
        require_equal(o1, o2);
    }
    
    {
        popt o1;
        popt_other o2{g_other_empty_value};
        
        require_equal(o1, o2);
    }
    
    {
        popt o1{g_empty_value};
        popt_other o2{g_other_empty_value};
        
        require_equal(o1, o2);
    }
    
    {
        popt o1{g_default_value};
        popt_other o2{g_default_value};
        
        require_equal(o1, o2);
    }

    {
        popt o1;
        popt_c o2;
        
        require_equal(o1, o2);
    }

    {
        popt o1;
        popt_c o2{g_empty_value_c};
        
        require_equal(o1, o2);
    }

    {
        popt o1{g_empty_value};
        popt_c o2{g_empty_value_c};
        
        require_equal(o1, o2);
    }

    {
        popt o1{g_default_value};
        popt_c o2{g_default_value_c};
        
        require_equal(o1, o2);
    }
}

TEST_CASE("Inequality") {
    {
        popt o1;
        popt o2{g_default_value};

        require_not_equal(o1, o2);
    }
    
    {
        popt o1{g_empty_value};
        popt o2{g_default_value};

        require_not_equal(o1, o2);
    }

    {
        popt o1{g_value};
        popt o2{g_default_value};
        
        require_not_equal(o1, o2);
    }

    {
        popt o1;
        popt_other o2{g_default_value};

        require_not_equal(o1, o2);
    }
    
    {
        popt o1{g_empty_value};
        popt_other o2{g_default_value};

        require_not_equal(o1, o2);
    }

    {
        popt o1{g_value};
        popt_other o2{g_default_value};
        
        require_not_equal(o1, o2);
    }

    {
        popt o1;
        popt_c o2{g_default_value_c};
        
        require_not_equal(o1, o2);
    }

    {
        popt o1{g_empty_value};
        popt_c o2{g_default_value_c};
        
        require_not_equal(o1, o2);
    }
}

TEST_CASE("Copy") {
    SUBCASE("From other packed_optional") {
        SUBCASE("Empty") {
            popt opt;
            popt copy;
            copy = opt;

            require_empty(copy);
            require_equal(opt, copy);
        }

        SUBCASE("With value") {
            popt opt{ g_value };
            popt copy;
            copy = opt;

            require_has_value(copy, g_value);
            require_equal(opt, copy);
        }
    }

    SUBCASE("From T") {
        SUBCASE("Empty") {
            popt copy;
            copy = g_empty_value;

            require_empty(copy);
        }

        SUBCASE("With value") {
            popt copy;
            copy = g_value;

            require_has_value(copy, g_value);
        }
    }
}