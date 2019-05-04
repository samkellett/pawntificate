#ifndef PAWNTIFICATE_BOARD_HPP
#define PAWNTIFICATE_BOARD_HPP

#include <array>
#include <string_view>

#include <iostream>

#include "cxx/make_array.hpp"

namespace pawntificate {

// based off the FEN notation, white is uppercase, black lower. 4 bits of
// information each, bit 0 is the colour and bits 1-3 are the type.
enum class colour : std::uint8_t {
  white = 0b0001u, black = 0b0000u
};

inline
auto operator<<(std::ostream &os, const colour &c) -> std::ostream & {
  switch(c) {
    case colour::white: return os << "w";
    case colour::black: return os << "b";
    default: return os << "colour<" << static_cast<unsigned>(c) << ">";
  }
}

constexpr auto flip_colour(colour &c) -> void{
  auto op = static_cast<std::uint8_t>(c);
  c = static_cast<colour>(op ^ 1u);
}

struct piece {
  constexpr piece(const std::uint8_t opcode) : opcode{opcode} {}

  std::uint8_t opcode;
};

constexpr auto operator==(const piece &lhs, const piece &rhs) -> bool {
  return lhs.opcode == rhs.opcode;
}

namespace pieces {

// null piece
constexpr piece _ = 0b0000u;

// white
constexpr piece P = 0b0011u, R = 0b0101u, N = 0b0111u, B = 0b1001u, Q = 0b1011u, K = 0b1101u;

// black
constexpr piece p = 0b0010u, r = 0b0100u, n = 0b0110u, b = 0b1000u, q = 0b1010u, k = 0b1100u;

} // namespace pieces

inline
auto operator<<(std::ostream &os, const piece &p_) -> std::ostream & {
  using namespace pieces;

  switch(p_.opcode) {
    case P.opcode: return os << 'P';
    case R.opcode: return os << 'R';
    case N.opcode: return os << 'N';
    case B.opcode: return os << 'B';
    case Q.opcode: return os << 'Q';
    case K.opcode: return os << 'K';
    case p.opcode: return os << 'p';
    case r.opcode: return os << 'r';
    case n.opcode: return os << 'n';
    case b.opcode: return os << 'b';
    case q.opcode: return os << 'q';
    case k.opcode: return os << 'k';
    case _.opcode: return os << '-';
    default: return os << "piece<" << static_cast<unsigned>(p_.opcode) << ">";
  }
}

constexpr auto to_promoted_piece(const char pawn, const colour c) -> piece {
  // p must be one of the pieces that a pawn can promote to, lower-case only.
  assert(pawn == 'r' || pawn == 'n' || pawn == 'b' || pawn == 'q');

  // get the piece as black as that colour bit is 0, we will OR in the actual
  // colour after.
  auto promoted_piece = [&]() -> piece {
    using namespace pieces;

    switch(pawn) {
      case 'r': return r;
      case 'n': return n;
      case 'b': return b;
      case 'q': return q;
    }

    assert(false && "unsupported piece promotion");
    return _;
  }();

  promoted_piece.opcode |= static_cast<std::uint8_t>(c);
  return promoted_piece;
}

constexpr auto is_pawn(const piece p) -> bool {
  return p == pieces::p || p == pieces::P;
}

enum class square : std::uint8_t {
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8,

  // null square
  _
};

std::ostream &operator<<(std::ostream &os, const square s) {
  if (s == square::_) {
    return os << "-";
  }

  const auto rank = static_cast<std::uint8_t>(s) / 8;
  const auto file = static_cast<std::uint8_t>(s) % 8;
  return os << char('a' + file) << char('1' + rank);
}

// the enum value for the squares start from zero so we utilise this to map
// ascii characters to their enum value quickly.
constexpr auto to_square(char file, char rank) -> square {
  assert(file >= 'a' && file <= 'h');
  assert(rank >= '1' && rank <= '8');

  // normalise
  file -= 'a';
  rank -= '1';

  // map to 1d index.
  const char index = file + rank * 8;
  return static_cast<square>(index);
}

// some tests, odd and even rank and file
static_assert(to_square('a', '1') == square::a1);
static_assert(to_square('c', '2') == square::c2);
static_assert(to_square('f', '7') == square::f7);
static_assert(to_square('h', '8') == square::h8);

// given two squares, return the amount of ranks apart they are.
constexpr auto rank_distance(const square from, const square to) -> std::uint8_t {
  const auto from_rank = static_cast<std::int8_t>(from) / 8;
  const auto to_rank = static_cast<std::int8_t>(to) / 8;

  const auto distance = from_rank - to_rank;
  return distance < 0 ? -distance : distance;
}

static_assert(rank_distance(square::a1, square::b1) == 0);
static_assert(rank_distance(square::a1, square::h1) == 0);
static_assert(rank_distance(square::b1, square::a1) == 0);
static_assert(rank_distance(square::h1, square::a1) == 0);
static_assert(rank_distance(square::a1, square::a2) == 1);
static_assert(rank_distance(square::a2, square::a1) == 1);
static_assert(rank_distance(square::h3, square::a4) == 1);
static_assert(rank_distance(square::a1, square::h8) == 7);

constexpr square move_by_rank(const square s, const std::int8_t distance) {
  const auto new_index = static_cast<std::uint8_t>(s) + distance * 8;
  return static_cast<square>(new_index);
}

static_assert(move_by_rank(square::a1, 1) == square::a2);
static_assert(move_by_rank(square::b2, 2) == square::b4);
static_assert(move_by_rank(square::c3, -1) == square::c2);

struct board {
  constexpr board() = default;
  board(const colour active, const std::array<piece, 64> piece_board, const square en_passant = square::_)
    : active(active), en_passant(en_passant), piece_board(piece_board) {}

  explicit constexpr board(const std::string_view move_list) {
    const auto set_square = [this](const square s, const piece p) {
      piece_board[static_cast<std::size_t>(s)] = p;
    };

    // move_list will have the format like: a1a2 b2b4 c5d6 a7a8q (promotion).
    // we don't do much error checking of the format for performance reasons.
    const auto end{std::end(move_list)};
    for (auto c = std::begin(move_list); c != end;) {
      flip_colour(active);

      assert(std::distance(c, end) >= 4);
      const auto from = to_square(*c, *(c + 1));
      const auto to = to_square(*(c + 2), *(c + 3));
      std::advance(c, 4);

      // is this a castling move?
      if (from == square::e1 && to == square::g1) {
        // white castle short
        set_square(square::e1, pieces::_);
        set_square(square::f1, pieces::r);
        set_square(square::g1, pieces::k);
        set_square(square::h1, pieces::_);
      } else if (from == square::e8 && to == square::g8) {
        // black castle short
        set_square(square::e8, pieces::_);
        set_square(square::f8, pieces::R);
        set_square(square::g8, pieces::K);
        set_square(square::h8, pieces::_);
      } else if (from == square::e1 && to == square::c1) {
        // white castle long
        set_square(square::e1, pieces::_);
        set_square(square::d1, pieces::r);
        set_square(square::c1, pieces::k);
        set_square(square::a1, pieces::_);
      } else if (from == square::e8 && to == square::c8) {
        // black castle long
        set_square(square::e8, pieces::_);
        set_square(square::d8, pieces::R);
        set_square(square::c8, pieces::K);
        set_square(square::a8, pieces::_);
      } else {
        auto &from_square = piece_board[std::size_t(from)];
        auto &to_square = piece_board[std::size_t(to)];

        // check for promotion, otherwise the piece is whatever was already at
        // the from square.
        const auto piece = [&] {
          if (c != end && *c != ' ') {
            return to_promoted_piece(*c++, active);
          } else {
            return from_square;
          }
        }();

        // if we have just moved onto the en passant square then remove the
        // pawn on the new rank.
        if (to == en_passant) {
          const auto pawn = move_by_rank(to, active == colour::white ? 1 : -1);
          set_square(pawn, pieces::_);
        }

        // if this is a 2 square pawn move set the en passant square
        if (is_pawn(piece) && rank_distance(to, from) == 2) {
          en_passant = move_by_rank(to, active == colour::white ? 1 : -1);
        } else {
          en_passant = square::_;
        }

        to_square = piece;
        from_square = pieces::_;
      }

      if (c != std::end(move_list)) {
        c++;
      }
    }
  }

  colour active = colour::white;
  square en_passant = square::_;

  // an internal representation of the chess board where each square is an index
  // into this array. white at the top of the board.
  // TODO: can make this half the size by packing pieces together.
  std::array<piece, 64> piece_board = [] {
    using namespace pieces;

    return cxx::make_array(
      r, n, b, q, k, b, n, r,
      p, p, p, p, p, p, p, p,
      _, _, _, _, _, _, _, _,
      _, _, _, _, _, _, _, _,
      _, _, _, _, _, _, _, _,
      _, _, _, _, _, _, _, _,
      P, P, P, P, P, P, P, P,
      R, N, B, Q, K, B, N, R
    );
  }();
};

inline
auto operator==(const board &lhs, const board &rhs) -> bool {
  return lhs.active == rhs.active
    && lhs.en_passant == rhs.en_passant
    && lhs.piece_board == rhs.piece_board;
}

inline
auto operator<<(std::ostream &os, const board &b) -> std::ostream & {
  os << b.piece_board[0];
  for (unsigned i = 1; i < b.piece_board.size(); ++i) {
    os << b.piece_board[i];

    if (i % 8 == 0) {
      os << "/";
    }
  }

  return os << " " << b.active << " " << b.en_passant;
}

} // namespace pawntificate

#endif // PAWNTIFICATE_BOARD_HPP
