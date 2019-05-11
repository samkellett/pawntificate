#include <gmock/gmock.h>

#include <pawntificate/board.hpp>

using namespace pawntificate::pieces;

using pawntificate::colour;
using pawntificate::move;
using pawntificate::ptype;
using pawntificate::square;

namespace pieces = pawntificate::pieces;

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
  }, square::g6);

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::a2, square::a3),
    move(square::a2, square::a4),
    move(square::a2, square::b3),

    move(square::c2, square::b3),

    move(square::c3, square::c4),

    move(square::d3, square::d4),

    move(square::a7, square::a8, ptype::rook),
    move(square::a7, square::a8, ptype::knight),
    move(square::a7, square::a8, ptype::bishop),
    move(square::a7, square::a8, ptype::queen),

    move(square::e7, square::d8, ptype::rook),
    move(square::e7, square::d8, ptype::knight),
    move(square::e7, square::d8, ptype::bishop),
    move(square::e7, square::d8, ptype::queen),

    move(square::f2, square::f3),
    move(square::f2, square::f4),
    move(square::f2, square::e3),
    move(square::f2, square::g3),

    move(square::h2, square::h3),
    move(square::h2, square::h4),
    move(square::h2, square::g3),

    move(square::h5, square::h6),
    move(square::h5, square::g6), // en passant

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
  }, square::g3);

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::a7, square::a6),
    move(square::a7, square::a5),
    move(square::a7, square::b6),

    move(square::c7, square::b6),

    move(square::c6, square::c5),

    move(square::d6, square::d5),

    move(square::e2, square::e1, ptype::rook),
    move(square::e2, square::e1, ptype::knight),
    move(square::e2, square::e1, ptype::bishop),
    move(square::e2, square::e1, ptype::queen),
    move(square::e2, square::d1, ptype::rook),
    move(square::e2, square::d1, ptype::knight),
    move(square::e2, square::d1, ptype::bishop),
    move(square::e2, square::d1, ptype::queen),

    move(square::f7, square::f6),
    move(square::f7, square::f5),
    move(square::f7, square::e6),
    move(square::f7, square::g6),

    move(square::h7, square::h6),
    move(square::h7, square::h5),
    move(square::h7, square::g6),

    move(square::h4, square::h3),
    move(square::h4, square::g3) // en passant
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
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::a1, square::b1),
    move(square::a1, square::c1),
    move(square::a1, square::a2),
    move(square::a1, square::a3),
    move(square::a1, square::a4),
    move(square::a1, square::a5),
    move(square::a1, square::a6),

    move(square::f3, square::e3),
    move(square::f3, square::g3),

    move(square::f7, square::d7),
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
    move(square::a1, square::c1),
    move(square::a1, square::a2),
    move(square::a1, square::a3),
    move(square::a1, square::a4),
    move(square::a1, square::a5),
    move(square::a1, square::a6),

    move(square::f3, square::f1),
    move(square::f3, square::f2),
    move(square::f3, square::f4),
    move(square::f3, square::f5),
    move(square::f3, square::f6),
    move(square::f3, square::e3),
    move(square::f3, square::g3),
    move(square::f3, square::h3),

    move(square::f7, square::d7),
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
    move(square::g4, square::f6),
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
    move(square::g4, square::f6),
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
    move(square::d4, square::e3),
    move(square::d4, square::e5),
    move(square::d4, square::f6),
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
    move(square::d4, square::e3),
    move(square::d4, square::e5),
    move(square::d4, square::f6),
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
    move(square::b2, square::e2),
    move(square::b2, square::a3),
    move(square::b2, square::b3),
    move(square::b2, square::c3),

    move(square::d4, square::c3),
    move(square::d4, square::d3),
    move(square::d4, square::d2),
    move(square::d4, square::d1),
    move(square::d4, square::e3),
    move(square::d4, square::e4),
    move(square::d4, square::f4),
    move(square::d4, square::g4),
    move(square::d4, square::h4),
    move(square::d4, square::e5),
    move(square::d4, square::f6),
    move(square::d4, square::d5),
    move(square::d4, square::d6),
    move(square::d4, square::c5),
    move(square::d4, square::b6),
    move(square::d4, square::a7),
    move(square::d4, square::c4),
    move(square::d4, square::b4)
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
    move(square::b2, square::e2),
    move(square::b2, square::a3),
    move(square::b2, square::b3),
    move(square::b2, square::c3),

    move(square::d4, square::c3),
    move(square::d4, square::d3),
    move(square::d4, square::d2),
    move(square::d4, square::d1),
    move(square::d4, square::e3),
    move(square::d4, square::e4),
    move(square::d4, square::f4),
    move(square::d4, square::g4),
    move(square::d4, square::h4),
    move(square::d4, square::e5),
    move(square::d4, square::f6),
    move(square::d4, square::d5),
    move(square::d4, square::d6),
    move(square::d4, square::c5),
    move(square::d4, square::b6),
    move(square::d4, square::a7),
    move(square::d4, square::c4),
    move(square::d4, square::b4)
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
  });

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::c4, square::b3),
    move(square::c4, square::c3),
    move(square::c4, square::c5),
    move(square::c4, square::b5),
    move(square::c4, square::b4)
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
  });

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

// bugs from real games

TEST(RealGame, CheckFromKnight) {
  // FEN: r1b1kbnr/pppp1ppp/8/4q3/1P6/2B4N/P1n1BPPP/RN1QK1R1 w Qkq - 0 11
  pawntificate::board uut("g1h3 e7e5 d2d4 e5d4 h1g1 d8f6 c1f4 d4d3 b2b4 f6d6 "
                          "e2e4 d3d2 f4d2 b8c6 e4e5 d6e5 f1e2 c6d4 d2c3 d4c2");

  const auto result = pawntificate::find_legal_moves(uut);
  ASSERT_THAT(result, UnorderedElementsAreArray({
    move(square::d1, square::c2),
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
    move(square::h3, square::g4),
    move(square::e2, square::e3),
    move(square::e2, square::d3),
    move(square::e2, square::f1)
  }));
}
