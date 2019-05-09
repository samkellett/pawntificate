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
  assert(pawn.type() == ptype::pawn);

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

auto find_legal_rook_moves(const square s,
                           const board &b,
                           move_out_iterator out) -> void {
  // legal rook moves:
  //   from where the piece is, walk up and down the file until you find another
  //   piece -- include that square if it is an enemy piece. ditto walking left
  //   and right on the rank.
  const auto rook_colour = get_piece(b, s).colour();
  const std::int8_t rook_rank = rank(s);
  const std::int8_t rook_file = file(s);

  // adds a rook move, returns false if a direction should not be continued
  const auto add_move = [&](const square new_square) -> bool {
    const auto other_piece = get_piece(b, new_square);
    if (other_piece == pieces::_) {
      *out++ = move{s, new_square};
      return true;
    } else if (rook_colour != other_piece.colour()) {
      *out++ = move{s, new_square};
      return false;
    } else {
      assert(rook_colour == other_piece.colour());
      return false;
    }
  };

  // up
  for (auto r = rook_rank - 1; r >= 0; --r) {
    const auto up = make_square(rook_file, r);
    if (!add_move(up)) {
      break;
    }
  }

  // down
  for (auto r = rook_rank + 1; r < 8; ++r) {
    const auto down = make_square(rook_file, r);
    if (!add_move(down)) {
      break;
    }
  }

  // left
  for (auto f = rook_file - 1; f >= 0; --f) {
    const auto left = make_square(f, rook_rank);
    if (!add_move(left)) {
      break;
    }
  }

  // right
  for (auto f = rook_file + 1; f < 8; ++f) {
    const auto right = make_square(f, rook_rank);
    if (!add_move(right)) {
      break;
    }
  }
}

auto find_legal_knight_moves(const square s,
                             const board &b,
                             move_out_iterator out) -> void {
  // legal knight moves:
  //   there is always 8 squares around the knight it can go as long as they
  //   are in the bounds of the board and not occupied by a friendly piece
  const auto knight_colour = get_piece(b, s).colour();

  using P = std::pair<std::int8_t, std::int8_t>;
  std::array<P, 8> offsets{
    P(1, 2), P(1, -2), P(2, 1), P(2, -1), P(-1, 2), P(-1, -2), P(-2, 1), P(-2, -1)
  };

  const std::int8_t r = rank(s);
  const std::int8_t f = file(s);

  for (const auto [ro, fo] : offsets) {
    if (r + ro > 7 || r + ro < 0 || f + fo > 7 || f + fo < 0) {
      continue;
    }

    const auto new_square = make_square(f + fo, r + ro);
    const auto other_piece = get_piece(b, new_square);
    if (other_piece == pieces::_ || other_piece.colour() != knight_colour) {
      *out++ = move{s, new_square};
    } else {
      assert(other_piece.colour() == knight_colour);
    }
  }
}

auto find_legal_bishop_moves(const square s,
                             const board &b,
                             move_out_iterator out) -> void {
  // legal bishop moves:
  //   similar to the rook moves, except it walks diagonally in each axis.
  const auto bishop_colour = get_piece(b, s).colour();
  const std::int8_t bishop_rank = rank(s);
  const std::int8_t bishop_file = file(s);

  // adds a bishop move, returns false if a direction should not be continued
  const auto add_move = [&](const square new_square) -> bool {
    const auto other_piece = get_piece(b, new_square);
    if (other_piece == pieces::_) {
      *out++ = move{s, new_square};
      return true;
    } else if (bishop_colour != other_piece.colour()) {
      *out++ = move{s, new_square};
      return false;
    } else {
      assert(bishop_colour == other_piece.colour());
      return false;
    }
  };

  // up-left
  for (auto r = bishop_rank - 1, f = bishop_file - 1; r >= 0 && f >= 0; --r, --f) {
    const auto upleft = make_square(f, r);
    if (!add_move(upleft)) {
      break;
    }
  }

  // up-right
  for (auto r = bishop_rank - 1, f = bishop_file + 1; r >= 0 && f < 8; --r, ++f) {
    const auto upright = make_square(f, r);
    if (!add_move(upright)) {
      break;
    }
  }

  // down-left
  for (auto r = bishop_rank + 1, f = bishop_file - 1; r < 8 && f >= 0; ++r, --f) {
    const auto downleft = make_square(f, r);
    if (!add_move(downleft)) {
      break;
    }
  }

  // down-right
  for (auto r = bishop_rank + 1, f = bishop_file + 1; r < 8 && f < 8; ++r, ++f) {
    const auto downright = make_square(f, r);
    if (!add_move(downright)) {
      break;
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
          find_legal_rook_moves(s, b, out);
          break;
        case ptype::knight:
          find_legal_knight_moves(s, b, out);
          break;
        case ptype::bishop:
          find_legal_bishop_moves(s, b, out);
          break;
        case ptype::queen:
          find_legal_rook_moves(s, b, out);
          find_legal_bishop_moves(s, b, out);
        case ptype::king:
          break;
      }
    }
  }

  return moves;
}

} // namespace pawntificate
