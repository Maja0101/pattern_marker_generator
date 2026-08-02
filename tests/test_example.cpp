#include <catch2/catch_amalgamated.hpp>

TEST_CASE("example test", "[health]") {
    CHECK(1 + 8 == 9);
    CHECK(8 + 1 == 9);
    CHECK(8 + 4 == 12);
}
