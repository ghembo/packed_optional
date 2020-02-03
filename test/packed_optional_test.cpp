#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "packed_optional/packed_optional.hpp"

namespace po = packed_optional;

TEST_CASE("Quick check") {
    po::packed_optional opt;
    REQUIRE(true);
}
