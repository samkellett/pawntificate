#include <gmock/gmock.h>

#include <pawntificate/board.hpp>

using namespace pawntificate::pieces;

using pawntificate::castle;
using pawntificate::colour;
using pawntificate::move;
using pawntificate::ptype;
using pawntificate::square;

namespace pieces = pawntificate::pieces;

using ::testing::AnyOf;
using ::testing::Contains;
using ::testing::Not;
using ::testing::UnorderedElementsAreArray;

TEST(FindLegalMoves, WhitePawns) {
  pawntificate::board uut(colour::white, {
    _, _, _, _, _, _, _, _,
    P, P, P, _, _, P, _, P,
    _, q, P, P, q, _, q, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, p, P,
    _, _, _, _, _, K, _, _,
    P, _, _, _, P, _, _, _,
    _, _, _, q, _, _, _, _
  }, castle::_, square::g6);

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::a2, square::a3),
    move(square::a2, square::a4),
    move(square::a2, square::b3, true),

    move(square::c2, square::b3, true),

    move(square::c3, square::c4),

    move(square::d3, square::d4),

    move(square::a7, square::a8, ptype::rook),
    move(square::a7, square::a8, ptype::knight),
    move(square::a7, square::a8, ptype::bishop),
    move(square::a7, square::a8, ptype::queen),

    move(square::e7, square::d8, ptype::rook, true),
    move(square::e7, square::d8, ptype::knight, true),
    move(square::e7, square::d8, ptype::bishop, true),
    move(square::e7, square::d8, ptype::queen, true),

    move(square::f2, square::f3),
    move(square::f2, square::f4),
    move(square::f2, square::e3, true),
    move(square::f2, square::g3, true),

    move(square::h2, square::h3),
    move(square::h2, square::h4),
    move(square::h2, square::g3, true),

    move(square::h5, square::h6),
    move(square::h5, square::g6, true), // en passant

    // legal king moves
    move(square::f6, square::f5),
    move(square::f6, square::g6),
    move(square::f6, square::g7)
  }));
}

TEST(FindLegalMoves, BlackPawns) {
  pawntificate::board uut(colour::black, {
    _, _, _, Q, _, _, _, _,
    _, _, _, _, p, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, P, p,
    _, _, _, _, _, _, _, _,
    _, Q, p, p, Q, _, Q, _,
    p, p, p, _, _, p, _, p,
    _, _, _, _, _, _, _, _
  }, castle::_, square::g3);

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::a7, square::a6),
    move(square::a7, square::a5),
    move(square::a7, square::b6, true),

    move(square::c7, square::b6, true),

    move(square::c6, square::c5),

    move(square::d6, square::d5),

    move(square::e2, square::e1, ptype::rook),
    move(square::e2, square::e1, ptype::knight),
    move(square::e2, square::e1, ptype::bishop),
    move(square::e2, square::e1, ptype::queen),
    move(square::e2, square::d1, ptype::rook, true),
    move(square::e2, square::d1, ptype::knight, true),
    move(square::e2, square::d1, ptype::bishop, true),
    move(square::e2, square::d1, ptype::queen, true),

    move(square::f7, square::f6),
    move(square::f7, square::f5),
    move(square::f7, square::e6, true),
    move(square::f7, square::g6, true),

    move(square::h7, square::h6),
    move(square::h7, square::h5),
    move(square::h7, square::g6, true),

    move(square::h4, square::h3),
    move(square::h4, square::g3, true) // en passant
  }));
}

TEST(FindLegalMoves, WhiteRook) {
  pawntificate::board uut(colour::white, {
    R, _, p, _, _, _, r, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, q, R, _, K,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    p, _, _, _, _, _, _, _,
    _, _, _, p, _, R, _, _,
    _, _, _, _, _, _, _, _
  }, castle::_);

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::a1, square::b1),
    move(square::a1, square::c1, true),
    move(square::a1, square::a2),
    move(square::a1, square::a3),
    move(square::a1, square::a4),
    move(square::a1, square::a5),
    move(square::a1, square::a6, true),

    move(square::f3, square::e3, true),
    move(square::f3, square::g3),

    move(square::f7, square::d7, true),
    move(square::f7, square::e7),
    move(square::f7, square::g7),
    move(square::f7, square::h7),
    move(square::f7, square::f4),
    move(square::f7, square::f5),
    move(square::f7, square::f6),
    move(square::f7, square::f8),

    // legal king moves
    move(square::h3, square::h2),
    move(square::h3, square::h4)
  }));
}

TEST(FindLegalMoves, BlackRook) {
  pawntificate::board uut(colour::black, {
    r, _, P, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, P, r, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    P, _, _, _, _, _, _, _,
    _, _, _, P, _, r, _, _,
    _, _, _, _, _, _, _, _
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::a1, square::b1),
    move(square::a1, square::c1, true),
    move(square::a1, square::a2),
    move(square::a1, square::a3),
    move(square::a1, square::a4),
    move(square::a1, square::a5),
    move(square::a1, square::a6, true),

    move(square::f3, square::f1),
    move(square::f3, square::f2),
    move(square::f3, square::f4),
    move(square::f3, square::f5),
    move(square::f3, square::f6),
    move(square::f3, square::e3, true),
    move(square::f3, square::g3),
    move(square::f3, square::h3),

    move(square::f7, square::d7, true),
    move(square::f7, square::e7),
    move(square::f7, square::g7),
    move(square::f7, square::h7),
    move(square::f7, square::f4),
    move(square::f7, square::f5),
    move(square::f7, square::f6),
    move(square::f7, square::f8)
  }));
}

TEST(FindLegalMoves, WhiteKnight) {
  pawntificate::board uut(colour::white, {
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, P, _, _, _,
    _, _, _, _, _, _, N, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, q, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::g4, square::f2),
    move(square::g4, square::h2),
    move(square::g4, square::h6),
    move(square::g4, square::f6, true),
    move(square::g4, square::e5),

    // legal pawn move
    move(square::e3, square::e4)
  }));
}

TEST(FindLegalMoves, BlackKnight) {
  pawntificate::board uut(colour::black, {
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, p, _, _, _,
    _, _, _, _, _, _, n, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, Q, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::g4, square::f2),
    move(square::g4, square::h2),
    move(square::g4, square::h6),
    move(square::g4, square::f6, true),
    move(square::g4, square::e5),

    // legal pawn move
    move(square::e3, square::e2)
  }));
}

TEST(FindLegalMoves, WhiteBishop) {
  pawntificate::board uut(colour::white, {
    _, _, _, _, _, _, _, _,
    _, B, _, _, _, _, _, _,
    _, _, _, _, p, _, _, _,
    _, _, _, B, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, p, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::b2, square::a1),
    move(square::b2, square::c1),
    move(square::b2, square::a3),
    move(square::b2, square::c3),

    move(square::d4, square::c3),
    move(square::d4, square::e3, true),
    move(square::d4, square::e5),
    move(square::d4, square::f6, true),
    move(square::d4, square::c5),
    move(square::d4, square::b6),
    move(square::d4, square::a7)
  }));
}

TEST(FindLegalMoves, BlackBishop) {
  pawntificate::board uut(colour::black, {
    _, _, _, _, _, _, _, _,
    _, b, _, _, _, _, _, _,
    _, _, _, _, P, _, _, _,
    _, _, _, b, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, P, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::b2, square::a1),
    move(square::b2, square::c1),
    move(square::b2, square::a3),
    move(square::b2, square::c3),

    move(square::d4, square::c3),
    move(square::d4, square::e3, true),
    move(square::d4, square::e5),
    move(square::d4, square::f6, true),
    move(square::d4, square::c5),
    move(square::d4, square::b6),
    move(square::d4, square::a7)
  }));
}

TEST(FindLegalMoves, WhiteQueen) {
  pawntificate::board uut(colour::white, {
    _, _, _, _, _, _, _, _,
    _, Q, _, _, p, _, _, _,
    _, p, _, _, p, _, _, _,
    _, p, _, Q, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, p, _, p, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::b2, square::a1),
    move(square::b2, square::b1),
    move(square::b2, square::c1),
    move(square::b2, square::a2),
    move(square::b2, square::c2),
    move(square::b2, square::d2),
    move(square::b2, square::e2, true),
    move(square::b2, square::a3),
    move(square::b2, square::b3, true),
    move(square::b2, square::c3),

    move(square::d4, square::c3),
    move(square::d4, square::d3),
    move(square::d4, square::d2),
    move(square::d4, square::d1),
    move(square::d4, square::e3, true),
    move(square::d4, square::e4),
    move(square::d4, square::f4),
    move(square::d4, square::g4),
    move(square::d4, square::h4),
    move(square::d4, square::e5),
    move(square::d4, square::f6, true),
    move(square::d4, square::d5),
    move(square::d4, square::d6, true),
    move(square::d4, square::c5),
    move(square::d4, square::b6),
    move(square::d4, square::a7),
    move(square::d4, square::c4),
    move(square::d4, square::b4, true)
  }));
}

TEST(FindLegalMoves, BlackQueen) {
  pawntificate::board uut(colour::black, {
    _, _, _, _, _, _, _, _,
    _, q, _, _, P, _, _, _,
    _, P, _, _, P, _, _, _,
    _, P, _, q, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, P, _, P, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::b2, square::a1),
    move(square::b2, square::b1),
    move(square::b2, square::c1),
    move(square::b2, square::a2),
    move(square::b2, square::c2),
    move(square::b2, square::d2),
    move(square::b2, square::e2, true),
    move(square::b2, square::a3),
    move(square::b2, square::b3, true),
    move(square::b2, square::c3),

    move(square::d4, square::c3),
    move(square::d4, square::d3),
    move(square::d4, square::d2),
    move(square::d4, square::d1),
    move(square::d4, square::e3, true),
    move(square::d4, square::e4),
    move(square::d4, square::f4),
    move(square::d4, square::g4),
    move(square::d4, square::h4),
    move(square::d4, square::e5),
    move(square::d4, square::f6, true),
    move(square::d4, square::d5),
    move(square::d4, square::d6, true),
    move(square::d4, square::c5),
    move(square::d4, square::b6),
    move(square::d4, square::a7),
    move(square::d4, square::c4),
    move(square::d4, square::b4, true)
  }));
}

TEST(FindLegalMoves, WhiteKing) {
  pawntificate::board uut(colour::white, {
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, n, K, _, k, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _
  }, castle::_);

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::c4, square::b3),
    move(square::c4, square::c3),
    move(square::c4, square::c5),
    move(square::c4, square::b5),
    move(square::c4, square::b4, true)
  }));
}

TEST(FindLegalMoves, DiscoveredChecks) {
  pawntificate::board uut(colour::white, {
    _, _, _, _, q, _, _, b,
    _, _, q, _, _, _, _, _,
    _, _, _, R, B, R, _, _,
    _, _, _, _, K, _, _, _,
    _, _, p, _, B, _, k, _,
    _, n, _, _, r, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _
  }, castle::_);

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_TRUE(result.empty());
}

TEST(FindLegalMoves, StartPos) {
  pawntificate::board uut;
  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    // pawns
    move(square::a2, square::a3),
    move(square::a2, square::a4),
    move(square::b2, square::b3),
    move(square::b2, square::b4),
    move(square::c2, square::c3),
    move(square::c2, square::c4),
    move(square::d2, square::d3),
    move(square::d2, square::d4),
    move(square::e2, square::e3),
    move(square::e2, square::e4),
    move(square::f2, square::f3),
    move(square::f2, square::f4),
    move(square::g2, square::g3),
    move(square::g2, square::g4),
    move(square::h2, square::h3),
    move(square::h2, square::h4),

    // knights
    move(square::b1, square::a3),
    move(square::b1, square::c3),
    move(square::g1, square::f3),
    move(square::g1, square::h3)
  }));
}

TEST(FindLegalMoves, e2e4) {
  pawntificate::board uut("e2e4");
  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    // pawns
    move(square::a7, square::a6),
    move(square::a7, square::a5),
    move(square::b7, square::b6),
    move(square::b7, square::b5),
    move(square::c7, square::c6),
    move(square::c7, square::c5),
    move(square::d7, square::d6),
    move(square::d7, square::d5),
    move(square::e7, square::e6),
    move(square::e7, square::e5),
    move(square::f7, square::f6),
    move(square::f7, square::f5),
    move(square::g7, square::g6),
    move(square::g7, square::g5),
    move(square::h7, square::h6),
    move(square::h7, square::h5),

    // knights
    move(square::b8, square::a6),
    move(square::b8, square::c6),
    move(square::g8, square::f6),
    move(square::g8, square::h6)
  }));
}

TEST(FindLegalMoves, CheckMate) {
  pawntificate::board uut(colour::black, {
    R, N, B, _, K, B, N, R,
    P, P, P, P, _, P, P, _,
    _, _, _, _, _, _, _, P,
    _, _, _, _, P, _, _, _,
    _, _, _, _, _, _, p, Q,
    _, _, _, _, _, p, _, _,
    p, p, p, p, p, _, _, p,
    r, n, b, q, k, b, n, r
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_TRUE(result.empty());
}

TEST(FindLegalMoves, BlockCheck) {
  pawntificate::board uut(colour::black, {
    R, N, B, Q, K, _, N, R,
    P, P, P, _, _, P, P, P,
    _, _, _, _, P, _, _, _,
    _, _, _, P, _, _, _, _,
    _, B, _, p, _, _, _, _,
    _, _, _, _, _, _, _, p,
    p, p, p, _, p, p, p, _,
    r, n, b, q, k, b, n, r
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::b8, square::c6),
    move(square::b8, square::d7),
    move(square::c8, square::d7),
    move(square::d8, square::d7),
    move(square::c7, square::c6)
  }));
}

TEST(FindLegalMoves, WhiteCastling) {
  pawntificate::board uut(colour::white, {
    R, _, _, _, K, _, _, R,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, Contains(move(square::e1, square::c1)));
  ASSERT_THAT(result, Contains(move(square::e1, square::g1)));
}

TEST(FindLegalMoves, WhiteCastlingBlockedByPiece) {
  pawntificate::board uut(colour::white, {
    R, _, _, Q, K, _, N, R,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, Not(Contains(AnyOf(
    move(square::e1, square::c1),
    move(square::e1, square::g1)
  ))));
}

TEST(FindLegalMoves, WhiteCastlingBlockedAttacker) {
  pawntificate::board uut(colour::white, {
    R, _, _, _, K, _, _, R,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, q, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, Not(Contains(AnyOf(
    move(square::e1, square::c1),
    move(square::e1, square::g1)
  ))));
}

TEST(FindLegalMoves, BlackCastling) {
  pawntificate::board uut(colour::black, {
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    r, _, _, _, k, _, _, r
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, Contains(move(square::e8, square::c8)));
  ASSERT_THAT(result, Contains(move(square::e8, square::g8)));
}

TEST(FindLegalMoves, BlackCastlingBlockedByPiece) {
  pawntificate::board uut(colour::black, {
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    r, _, _, q, k, _, n, r
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, Not(Contains(AnyOf(
    move(square::e8, square::c8),
    move(square::e8, square::g8)
  ))));
}

TEST(FindLegalMoves, BlackCastlingBlockedAttacker) {
  pawntificate::board uut(colour::black, {
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, Q, _, _, _, _,
    _, _, _, _, _, _, _, _,
    _, _, _, _, _, _, _, _,
    r, _, _, _, k, _, _, r
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, Not(Contains(AnyOf(
    move(square::e8, square::c8),
    move(square::e8, square::g8)
  ))));
}

// bugs from real games

TEST(RealGame, CheckFromKnight) {
  // FEN: r1b1kbnr/pppp1ppp/8/4q3/1P6/2B4N/P1n1BPPP/RN1QK1R1 w Qkq - 0 11
  pawntificate::board uut("g1h3 e7e5 d2d4 e5d4 h1g1 d8f6 c1f4 d4d3 b2b4 f6d6 "
                          "e2e4 d3d2 f4d2 b8c6 e4e5 d6e5 f1e2 c6d4 d2c3 d4c2");

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::d1, square::c2, true),
    move(square::e1, square::d2),
    move(square::e1, square::f1)
  }));
}

TEST(RealGame, CheckFromPawn) {
  // FEN: r3k1nr/8/3b3B/pN2p2P/B5b1/R3p2P/3pK3/1R6 w kq - 2 38
  pawntificate::board uut("e2e3 e7e5 g1h3 b8c6 h3g1 d7d5 f1a6 b7b5 h2h3 c6b8 "
                          "f2f4 c7c5 a6b5 c8d7 d1e2 d8h4 e1d1 h4e1 d1e1 f8d6 "
                          "g1f3 f7f5 b5a4 g7g5 g2g4 h7h5 e2g2 a7a5 b1c3 b8a6 "
                          "g4h5 g5f4 g2g3 f4e3 c3b5 e3d2 f3d2 a6b4 e1e2 b4c2 "
                          "d2e4 c2d4 e2f1 d4f3 c1h6 f3h2 f1g1 h2g4 a2a3 g4e3 "
                          "b2b4 e3d1 g1g2 d1c3 a1b1 c3e2 g2f3 e2g3 f3g3 f5e4 "
                          "b1b3 c5b4 h1f1 b4a3 g3f2 d5d4 f1b1 d4d3 b3a3 e4e3 "
                          "f2e1 d3d2 e1e2 d7g4");

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::h3, square::g4, true),

    move(square::e2, square::e3, true),
    move(square::e2, square::d3),
    move(square::e2, square::f1)
  }));
}

TEST(RealGame, EnemyKingProtectsEscapeSquare1) {
  // FEN: 8/2p3p1/6k1/p1p5/4pK2/5q2/8/8 w - - 2 57
  pawntificate::board uut("f2f3 e7e5 g2g3 f8c5 e2e4 d7d5 e4d5 c5g1 h1g1 d8d5 "
                          "h2h3 d5d4 g1h1 c8f5 a2a4 g8f6 a4a5 e8g8 f1g2 f8e8 "
                          "c2c3 d4c4 b1a3 c4a6 d1a4 f5d3 h3h4 a6c6 a4c6 b8c6 "
                          "a3b1 e8e7 g2h3 a8d8 b2b3 d3b1 a1b1 c6a5 c3c4 d8d3 "
                          "c1a3 e7e8 b1c1 d3b3 c1c3 b3c3 d2c3 a5c4 a3c5 b7b6 "
                          "h3f1 c4d2 e1d2 b6c5 h1h2 e5e4 f3e4 f6e4 d2d3 e4g3 "
                          "h2f2 e8e4 h4h5 e4h4 f1e2 g3e2 f2e2 h7h6 c3c4 h4h5 "
                          "d3e3 h5e5 e3d3 e5e6 e2e6 f7e6 d3c2 e6e5 c2c3 a7a6 "
                          "c3b3 e5e4 b3c3 g8h7 c3d2 h7g6 d2c1 g6h7 c1d1 h7g6 "
                          "d1c2 h6h5 c2c1 h5h4 c1d2 h4h3 d2c1 h3h2 c1d2 h2h1q "
                          "d2e2 h1c1 e2f2 c1c4 f2e3 c4c2 e3f4 c2d3 f4e5 a6a5 "
                          "e5f4 d3f3");

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::f4, square::e5)
  }));
}

TEST(RealGame, EnemyKingProtectsEscapeSquare2) {
  // FEN: 8/8/3R4/8/4k1P1/5N1P/5K2/8 b - - 0 54
  pawntificate::board uut("d2d4 h7h6 e2e4 g7g5 f1d3 b8c6 d4d5 c6e5 f2f4 e5d3 "
                          "d1d3 g5f4 d3d4 f4f3 g1f3 h8h7 e1g1 f7f6 f1d1 d7d6 "
                          "c1e3 f8g7 d4a4 e8f7 f3d2 d8e8 a4e8 f7e8 b1a3 e8d8 "
                          "e3d4 f6f5 d4g7 h7g7 d1f1 g7f7 e4f5 f7f5 d2e4 e7e6 "
                          "d5e6 f5f1 a1f1 d8e8 a3b5 a8b8 b5c7 e8e7 f1f7 e7d8 "
                          "c7d5 c8e6 f7f8 d8d7 d5b6 d7c7 b6a8 b8a8 f8a8 e6a2 "
                          "b2b3 a2b3 c2b3 g8e7 a8a7 e7f5 b3b4 c7b6 a7a5 f5d4 "
                          "a5d5 d4c2 d5d6 b6c7 d6h6 c2b4 e4c5 b7b6 h6h7 c7b8 "
                          "h7b7 b8c8 b7b6 b4d5 b6b5 d5e3 h2h3 e3d1 b5b3 c8d8 "
                          "b3d3 d8e7 d3d1 e7f6 d1d7 f6g5 d7d6 g5f5 c5d3 f5g5 "
                          "d3e5 g5f5 e5f3 f5f4 g1f2 f4e4 g2g4");

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::e4, square::f4)
  }));
}
