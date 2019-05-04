#include "pawntificate/board.hpp"

namespace pawntificate {

namespace {

using move_out_iterator = std::back_insert_iterator<std::vector<move>>;

auto get_piece(const board &b, const square s) -> piece {
  const auto id = static_cast<std::size_t>(s);
  assert(id < b.piece_board.size());

  return b.piece_board[id];
}

auto find_legal_pawn_moves(const square s,
                           const board &b,
                           move_out_iterator out) -> void {
  // legal pawn moves:
  //   if the target square is empty:
  //    - one move up,
  //    - one move up with a promotion if on the 7th rank,
  //    - two moves up if on the 2nd rank and intermediate square is empty,
  //   diagonally up if an enemy piece is on that square or that square is the
  //    en passant square.
  const auto pawn = get_piece(b, s);
  const std::int8_t direction = pawn.colour() == colour::white ? 1 : -1;

  const std::uint8_t second_rank = pawn.colour() == colour::white ? 1 : 6;

  const auto add_moves = [&](const square new_square) {
    const std::uint8_t top_rank = pawn.colour() == colour::white ? 7 : 0;

    if (rank(new_square) == top_rank) {
      *out++ = move{s, new_square, pieces::r};
      *out++ = move{s, new_square, pieces::k};
      *out++ = move{s, new_square, pieces::b};
      *out++ = move{s, new_square, pieces::q};
    } else {
      *out++ = move{s, new_square};
    }
  };

  // move up one
  const auto up1 = move_by_rank(s, direction);
  if (get_piece(b, up1) == pieces::_) {
    add_moves(up1);

    // move up two
    if (rank(s) == second_rank) {
      const auto up2 = move_by_rank(up1, direction);
      if (get_piece(b, up2) == pieces::_) {
        *out++ = move{s, up2};
      }
    }
  }

  const auto can_take = [&](const square new_square) {
    const auto other_piece = get_piece(b, new_square);
    return (other_piece != pieces::_ && other_piece.colour() != pawn.colour())
      || new_square == b.en_passant;
  };

  // take a piece left
  if (file(up1) > 0) {
    const auto up1left1 = move_by_file(up1, -1);
    if (can_take(up1left1)) {
      add_moves(up1left1);
    }
  }

  // take a piece right
  if (file(up1) < 7) {
    const auto up1right1 = move_by_file(up1, 1);
    if (can_take(up1right1)) {
      add_moves(up1right1);
    }
  }
}

} // unnamed namespace

auto find_legal_moves([[maybe_unused]] const board &b) -> std::vector<move> {
  std::vector<move> moves;
  auto out = std::back_inserter(moves);

  // walk through board, for each piece who's turn it is populate the moves
  // vector with their legal moves.
  for (unsigned i = 0; i < b.piece_board.size(); ++i) {
    const auto p = b.piece_board[i];
    const auto s = static_cast<square>(i);

    if (p.colour() == b.active) {
      switch(p.type()) {
        case ptype::pawn:
          find_legal_pawn_moves(s, b, out);
          break;
        case ptype::rook:
        case ptype::knight:
        case ptype::bishop:
        case ptype::queen:
        case ptype::king:
          break;
      }
    }
  }

  return moves;
}

} // namespace pawntificate
