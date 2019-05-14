#include <gtest/gtest.h>

#include <pawntificate/board.hpp>
#include <pawntificate/evaluate.hpp>

using namespace std::literals;

using pawntificate::move;
using pawntificate::square;

TEST(Evaluate, MaximisingCorrectPlayer) {
  // white's rook is hanging, test that the evaluation is the correct way around
  // by checking that the rook is taken.
  // 1. a4 { A00 Ware Opening } a6 2. b4 e6 3. b5 axb5 4. axb5 Rxa1
  pawntificate::board uut("a2a4 a7a6 b2b4 e7e6 b4b5 a6b5 a4b5");
  const auto result = pawntificate::evaluate(uut);
  ASSERT_EQ(result, move(square::a8, square::a1));
}

TEST(Evaluate, MateInOne) {
  // scholar's mate: 1. e4 e5 2. Qf3 Nc6 3. Bc4 Bc5 4. Qxf7#
  pawntificate::board uut("e2e4 e7e5 d1f3 b8c6 f1c4 f8c5");
  const auto result = pawntificate::evaluate(uut);
  ASSERT_EQ(result, move(square::f3, square::f7));
}

// bugs from real games

TEST(RealGame, OnlyLegalMove) {
  // FEN: 2r1k1nr/8/1pp5/3p1p2/4nPp1/5P2/PPPP1qPP/R1B1K1NR w Kk - 1 23
  pawntificate::board uut("b1c3 e7e6 c3b5 d7d6 a1b1 c7c6 b5d4 b7b6 b1a1 a7a6 "
                          "a1b1 f7f6 b1a1 e6e5 d4f3 d6d5 e2e3 f6f5 f3e5 d8f6 "
                          "d1h5 g7g6 e5g6 h7g6 h5f3 b8d7 a1b1 d7c5 b1a1 c5e4 "
                          "a1b1 g6g5 b1a1 g5g4 f3f4 f8d6 f1a6 d6f4 a6c8 a8c8 "
                          "e3f4 f6d4 f2f3 d4f2");

  const auto result = pawntificate::evaluate(uut);
  ASSERT_EQ(result, move(square::e1, square::d1));
}
