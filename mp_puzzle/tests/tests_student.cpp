#include <catch2/catch_test_macros.hpp>
#include "puzzle.h"

// You may write your own test cases in this file to test your code.
// Test cases in this file are not graded.

TEST_CASE("My Test Case", "") {
    bool student_wrote_test_case = false;

    // < 5 6 1 3 2 8 7 11 13 9 15 12 10 0 14 4 >
    // < 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0 >
    PuzzleState start = PuzzleState({5, 6, 1, 3, 2, 8, 7, 11, 13, 9, 15, 12, 10, 0, 14, 4});
    PuzzleState end = PuzzleState();

    size_t iterations;

    solveAstar(start, end, &iterations);



    REQUIRE(true);
}