#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "packed_optional/packed_optional.hpp"

namespace po = packed_optional;
constexpr int g_empty_value{ -1 };
constexpr int g_default_value{ 42 };
constexpr int g_value{ 10 };
using popt = po::packed_optional<int, g_empty_value>;

template<typename T, T empty_value>
void require_empty(po::packed_optional<T, empty_value> opt) {
    REQUIRE_FALSE(opt.has_value());
    REQUIRE_FALSE(opt);

    REQUIRE_THROWS(opt.value());
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

TEST_CASE("Copy") {
    SUBCASE("From other packed_optional") {
        SUBCASE("Empty") {
            popt opt;
            popt copy;
            copy = opt;

            require_empty(copy);
        }

        SUBCASE("With value") {
            popt opt{ g_value };
            popt copy;
            copy = opt;

            require_has_value(copy, g_value);
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