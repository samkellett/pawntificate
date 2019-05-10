#include <gtest/gtest.h>

#include <pawntificate/board.hpp>

using namespace std::literals;

using namespace pawntificate::pieces;

using pawntificate::colour;
using pawntificate::move;
using pawntificate::ptype;
using pawntificate::square;

class Moves : public ::testing::TestWithParam<std::pair<move, std::string_view>> {};

TEST_P(Moves, StringRepresentation) {
  const auto [uut, expected] = GetParam();

  std::stringstream ss;
  ss << uut;

  ASSERT_EQ(ss.str(), expected);
}

INSTANTIATE_TEST_SUITE_P(SomeMoves, Moves, ::testing::Values(
  std::make_pair(move{square::e2, square::e4}, "e2e4"sv),
  std::make_pair(move{square::h7, square::h8, ptype::queen}, "h7h8q"sv),
  std::make_pair(move{square::h7, square::h8, ptype::knight}, "h7h8n"sv)
));

TEST(BoardState, DefaultConstructed) {
  pawntificate::board uut;
  ASSERT_EQ(uut, pawntificate::board(colour::white, {
    R, N, B, Q, K, B, N, R,
    P, P, P, P, P, P, P, P,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    p, p, p, p, p, p, p, p,
    r, n, b, q, k, b, n, r
  }));
}

TEST(BoardState, NoMoves) {
  pawntificate::board uut("");
  ASSERT_EQ(uut, pawntificate::board{});
}

TEST(BoardState, SingleMove) {
  pawntificate::board uut("e2e4");
  ASSERT_EQ(uut, pawntificate::board(colour::black, {
    R, N, B, Q, K, B, N, R,
    P, P, P, P, _, P, P, P,
    _, _, _, _, _, _, _, _,
    _, _, _, _, P, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    p, p, p, p, p, p, p, p,
    r, n, b, q, k, b, n, r
  }, square::e3));
}

TEST(BoardState, TwoMoves) {
  pawntificate::board uut("e2e4 c7c5");
  ASSERT_EQ(uut, pawntificate::board(colour::white, {
    R, N, B, Q, K, B, N, R,
    P, P, P, P, _, P, P, P,
    _, _, _, _, _, _, _, _,
    _, _, _, _, P, _, _, _,
    _, _, p, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    p, p, _, p, p, p, p, p,
    r, n, b, q, k, b, n, r
  }, square::c6));
}

TEST(BoardState, Capture) {
  pawntificate::board uut("e2e4 d7d5 e4d5");
  ASSERT_EQ(uut, pawntificate::board(colour::black, {
    R, N, B, Q, K, B, N, R,
    P, P, P, P, _, P, P, P,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, P, _, _, _, _,
    _, _, _, _, _, _, _, _,
    p, p, p, _, p, p, p, p,
    r, n, b, q, k, b, n, r
  }));
}

TEST(BoardState, WhiteQueenPromotion) {
  pawntificate::board uut("e2e4 d7d5 e4d5 g8f6 f1b5 c7c6 d5c6 d8b6 c6b7 b6b5 b7c8q");
  ASSERT_EQ(uut, pawntificate::board(colour::black, {
    R, N, B, Q, K, _, N, R,
    P, P, P, P, _, P, P, P,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, q, _, _, _, _, _, _,
    _, _, _, _, _, n, _, _,
    p, _, _, _, p, p, p, p,
    r, n, Q, _, k, b, _, r
  }));
}

TEST(BoardState, BlackRookPromotion) {
  pawntificate::board uut("c2c3 b7b5 d1b3 b5b4 b3d5 b4c3 d5a8 c3b2 e1d1 b2c1r d1e1");
  ASSERT_EQ(uut, pawntificate::board(colour::black, {
    R, N, r, _, K, B, N, R,
    P, _, _, P, P, P, P, P,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    p, _, p, p, p, p, p, p,
    Q, n, b, q, k, b, n, r
  }));
}

TEST(BoardState, CastlingShort) {
  pawntificate::board uut("e2e4 e7e5 g1f3 g8f6 f1c4 f8c5 e1g1 e8g8");
  ASSERT_EQ(uut, pawntificate::board(colour::white, {
    R, N, B, Q, _, R, K, _,
    P, P, P, P, _, P, P, P,
    _, _, _, _, _, N, _, _,
    _, _, B, _, P, _, _, _,
    _, _, b, _, p, _, _, _,
    _, _, _, _, _, n, _, _,
    p, p, p, p, _, p, p, p,
    r, n, b, q, _, r, k, _
  }));
}

TEST(BoardState, CastlingLong) {
  pawntificate::board uut("d2d4 d7d5 b1c3 b8c6 c1e3 c8e6 d1d2 d8d7 e1c1 e8c8");
  ASSERT_EQ(uut, pawntificate::board(colour::white, {
    _, _, K, R, _, B, N, R,
    P, P, P, Q, P, P, P, P,
    _, _, N, _, B, _, _, _,
    _, _, _, P, _, _, _, _,
    _, _, _, p, _, _, _, _,
    _, _, n, _, b, _, _, _,
    p, p, p, q, p, p, p, p,
    _, _, k, r, _, b, n, r
  }));
}

TEST(BoardState, EnPassantWhite) {
  pawntificate::board uut("b2b4 g7g5 b4b5 g5g4 f2f4 c7c5 b5c6");
  ASSERT_EQ(uut, pawntificate::board(colour::black, {
    R, N, B, Q, K, B, N, R,
    P, _, P, P, P, _, P, P,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, P, p, _,
    _, _, _, _, _, _, _, _,
    _, _, P, _, _, _, _, _,
    p, p, _, p, p, p, _, p,
    r, n, b, q, k, b, n, r
  }));
}

TEST(BoardState, EnPassantBlack) {
  pawntificate::board uut("b2b4 g7g5 b4b5 g5g4 f2f4 g4f3");
  ASSERT_EQ(uut, pawntificate::board(colour::white, {
    R, N, B, Q, K, B, N, R,
    P, _, P, P, P, _, P, P,
    _, _, _, _, _, p, _, _,
    _, _, _, _, _, _, _, _,
    _, P, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    p, p, p, p, p, p, _, p,
    r, n, b, q, k, b, n, r
  }));
}

TEST(BoardState, NonPawnEnPassantSquare) {
  pawntificate::board uut("e2e4 a7a6 d1g4 e7e5 g4e6");
  ASSERT_EQ(uut, pawntificate::board(colour::black, {
    R, N, B, _, K, B, N, R,
    P, P, P, P, _, P, P, P,
    _, _, _, _, _, _, _, _,
    _, _, _, _, P, _, _, _,
    _, _, _, _, p, _, _, _,
    p, _, _, _, Q, _, _, _,
    _, p, p, p, _, p, p, p,
    r, n, b, q, k, b, n, r
  }));
}
