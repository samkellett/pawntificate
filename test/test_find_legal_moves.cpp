#include <gmock/gmock.h>

#include <pawntificate/board.hpp>

using namespace pawntificate::pieces;

using pawntificate::colour;
using pawntificate::move;
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

    move(square::a7, square::a8, pieces::r),
    move(square::a7, square::a8, pieces::k),
    move(square::a7, square::a8, pieces::b),
    move(square::a7, square::a8, pieces::q),

    move(square::e7, square::d8, pieces::r),
    move(square::e7, square::d8, pieces::k),
    move(square::e7, square::d8, pieces::b),
    move(square::e7, square::d8, pieces::q),

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

    move(square::e2, square::e1, pieces::r),
    move(square::e2, square::e1, pieces::k),
    move(square::e2, square::e1, pieces::b),
    move(square::e2, square::e1, pieces::q),
    move(square::e2, square::d1, pieces::r),
    move(square::e2, square::d1, pieces::k),
    move(square::e2, square::d1, pieces::b),
    move(square::e2, square::d1, pieces::q),

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
