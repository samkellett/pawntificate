#include <gtest/gtest.h>

#include <pawntificate/board.hpp>

using namespace pawntificate::pieces;

using pawntificate::colour;
using pawntificate::square;

TEST(BoardState, DefaultConstructed) {
  pawntificate::board uut;
  ASSERT_EQ(uut, pawntificate::board(colour::white, {
    r, n, b, q, k, b, n, r,
    p, p, p, p, p, p, p, p,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    P, P, P, P, P, P, P, P,
    R, N, B, Q, K, B, N, R
  }));
}

TEST(BoardState, NoMoves) {
  pawntificate::board uut("");
  ASSERT_EQ(uut, pawntificate::board{});
}

TEST(BoardState, SingleMove) {
  pawntificate::board uut("e2e4");
  ASSERT_EQ(uut, pawntificate::board(colour::black, {
    r, n, b, q, k, b, n, r,
    p, p, p, p, _, p, p, p,
    _, _, _, _, _, _, _, _,
    _, _, _, _, p, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    P, P, P, P, P, P, P, P,
    R, N, B, Q, K, B, N, R
  }, square::e3));
}

TEST(BoardState, TwoMoves) {
  pawntificate::board uut("e2e4 c7c5");
  ASSERT_EQ(uut, pawntificate::board(colour::white, {
    r, n, b, q, k, b, n, r,
    p, p, p, p, _, p, p, p,
    _, _, _, _, _, _, _, _,
    _, _, _, _, p, _, _, _,
    _, _, P, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    P, P, _, P, P, P, P, P,
    R, N, B, Q, K, B, N, R
  }, square::c6));
}

TEST(BoardState, Capture) {
  pawntificate::board uut("e2e4 d7d5 e4d5");
  ASSERT_EQ(uut, pawntificate::board(colour::black, {
    r, n, b, q, k, b, n, r,
    p, p, p, p, _, p, p, p,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, p, _, _, _, _,
    _, _, _, _, _, _, _, _,
    P, P, P, _, P, P, P, P,
    R, N, B, Q, K, B, N, R
  }));
}

TEST(BoardState, WhiteQueenPromotion) {
  pawntificate::board uut("e2e4 d7d5 e4d5 g8f6 f1b5 c7c6 d5c6 d8b6 c6b7 b6b5 b7c8q");
  ASSERT_EQ(uut, pawntificate::board(colour::black, {
    r, n, b, q, k, _, n, r,
    p, p, p, p, _, p, p, p,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, Q, _, _, _, _, _, _,
    _, _, _, _, _, N, _, _,
    P, _, _, _, P, P, P, P,
    R, N, q, _, K, B, _, R
  }));
}

TEST(BoardState, BlackRookPromotion) {
  pawntificate::board uut("c2c3 b7b5 d1b3 b5b4 b3d5 b4c3 d5a8 c3b2 e1d1 b2c1r d1e1");
  ASSERT_EQ(uut, pawntificate::board(colour::black, {
    r, n, R, _, k, b, n, r,
    p, _, _, p, p, p, p, p,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    P, _, P, P, P, P, P, P,
    q, N, B, Q, K, B, N, R
  }));
}

TEST(BoardState, CastlingShort) {
  pawntificate::board uut("e2e4 e7e5 g1f3 g8f6 f1c4 f8c5 e1g1 e8g8");
  ASSERT_EQ(uut, pawntificate::board(colour::white, {
    r, n, b, q, _, r, k, _,
    p, p, p, p, _, p, p, p,
    _, _, _, _, _, n, _, _,
    _, _, b, _, p, _, _, _,
    _, _, B, _, P, _, _, _,
    _, _, _, _, _, N, _, _,
    P, P, P, P, _, P, P, P,
    R, N, B, Q, _, R, K, _
  }));
}

TEST(BoardState, CastlingLong) {
  pawntificate::board uut("d2d4 d7d5 b1c3 b8c6 c1e3 c8e6 d1d2 d8d7 e1c1 e8c8");
  ASSERT_EQ(uut, pawntificate::board(colour::white, {
    _, _, k, r, _, b, n, r,
    p, p, p, q, p, p, p, p,
    _, _, n, _, b, _, _, _,
    _, _, _, p, _, _, _, _,
    _, _, _, P, _, _, _, _,
    _, _, N, _, B, _, _, _,
    P, P, P, Q, P, P, P, P,
    _, _, K, R, _, B, N, R
  }));
}

TEST(BoardState, EnPassantWhite) {
  pawntificate::board uut("b2b4 g7g5 b4b5 g5g4 f2f4 c7c5 b5c6");
  ASSERT_EQ(uut, pawntificate::board(colour::black, {
    r, n, b, q, k, b, n, r,
    p, _, p, p, p, _, p, p,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, p, P, _,
    _, _, _, _, _, _, _, _,
    _, _, p, _, _, _, _, _,
    P, P, _, P, P, P, _, P,
    R, N, B, Q, K, B, N, R
  }));
}

TEST(BoardState, EnPassantBlack) {
  pawntificate::board uut("b2b4 g7g5 b4b5 g5g4 f2f4 g4f3");
  ASSERT_EQ(uut, pawntificate::board(colour::white, {
    r, n, b, q, k, b, n, r,
    p, _, p, p, p, _, p, p,
    _, _, _, _, _, P, _, _,
    _, _, _, _, _, _, _, _,
    _, p, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    P, P, P, P, P, P, _, P,
    R, N, B, Q, K, B, N, R
  }));
}
