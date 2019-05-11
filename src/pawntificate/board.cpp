#include "pawntificate/board.hpp"

namespace pawntificate {

namespace {

using P = std::pair<std::int8_t, std::int8_t>;
constexpr std::array<P, 8> knight_offsets{
  P(1, 2), P(1, -2), P(2, 1), P(2, -1), P(-1, 2), P(-1, -2), P(-2, 1), P(-2, -1)
};

auto get_piece(const board &b, const square s) -> piece {
  const auto id = static_cast<std::size_t>(s);
  assert(id < b.piece_board.size());

  return b.piece_board[id];
}

auto find_king(const board &b) -> square {
  for (unsigned i = 0; i < b.piece_board.size(); ++i) {
    const auto p = b.piece_board[i];
    if (p.type() == ptype::king && p.colour() == b.active) {
      return static_cast<square>(i);
    }
  }

  return square::_;
}

// from the king, move outwards in each direction until either a piece or the
// edge of the board is hit. if it is a piece and that piece is not the same
// colour as the king then this move cannot be played. this includes when the
// king is already in check and this move does not help or if this move is
// why the king is now in check. we may want to optimise the case that the king
// is already in check later but for now it's convenient that this always works
auto king_is_safe(const board &b, const square king, const square from, const square to) -> bool {
  // if no king he can't be in danger. perhaps assert against this?
  if (king == square::_) {
    return true;
  }

  const std::int8_t king_rank = rank(king);
  const std::int8_t king_file = file(king);

  enum class safe : std::uint8_t {
    yes, no, unknown
  };

  const auto is_safe = [&](const square new_square, const auto is_dangerous_piece) -> safe {
    // if we reach the square we care about we are safe.
    if (new_square == to) {
      return safe::yes;
    }

    // if we hit an empty square, including the one that will soon be empty then
    // continue searching.
    const auto p = get_piece(b, new_square);
    if (p == pieces::_ || new_square == from) {
      return safe::unknown;
    }

    // otherwise we must have hit an actual piece. if it's our colour we are ok,
    // if not we have to check which piece it is to see if it can attack the king
    // from where it is.
    return p.colour() == b.active || !is_dangerous_piece(p, new_square) ? safe::yes : safe::no;
  };

  const auto dangerous_straight_piece = [&](const piece p, const square s) {
    // a dangerous straight piece is a rook or a queen, or a king if it is within
    // one rank.
    return p.type() == ptype::queen ||
           p.type() == ptype::rook ||
           (p.type() == ptype::king && file_distance(king, s) == 1);
  };

  const auto dangerous_diagonal_piece = [&](const piece p, const square s) {
    // a dangerous diagonal piece is a bishop or a queen, or a king if it is within
    // one file and one rank or a pawn on the correct side (so that the pawn attacks "up")
    const std::int8_t enemy_pawn_direction = b.active != colour::white ? -1 : 1;

    return p.type() == ptype::queen ||
           p.type() == ptype::bishop ||
           (p.type() == ptype::king && file_distance(king, s) == 1 && rank_distance(king, s) == 1) ||
           (p.type() == ptype::pawn && file_distance(king, s) == 1 && signed_rank_distance(king, s) == enemy_pawn_direction);
  };

  // up
  for (auto r = king_rank - 1; r >= 0; --r) {
    const auto up = make_square(king_file, r);
    if (const auto s = is_safe(up, dangerous_straight_piece); s != safe::unknown) {
      if (s == safe::yes) {
        break;
      } else {
        return false;
      }
    }
  }

  // down
  for (auto r = king_rank + 1; r < 8; ++r) {
    const auto down = make_square(king_file, r);
    if (const auto s = is_safe(down, dangerous_straight_piece); s != safe::unknown) {
      if (s == safe::yes) {
        break;
      } else {
        return false;
      }
    }
  }

  // left
  for (auto f = king_file - 1; f >= 0; --f) {
    const auto left = make_square(f, king_rank);
    if (const auto s = is_safe(left, dangerous_straight_piece); s != safe::unknown) {
      if (s == safe::yes) {
        break;
      } else {
        return false;
      }
    }
  }

  // right
  for (auto f = king_file + 1; f < 8; ++f) {
    const auto right = make_square(f, king_rank);
    if (const auto s = is_safe(right, dangerous_straight_piece); s != safe::unknown) {
      if (s == safe::yes) {
        break;
      } else {
        return false;
      }
    }
  }

  // up-left
  for (auto r = king_rank - 1, f = king_file - 1; r >= 0 && f >= 0; --r, --f) {
    const auto upleft = make_square(f, r);
    if (const auto s = is_safe(upleft, dangerous_diagonal_piece); s != safe::unknown) {
      if (s == safe::yes) {
        break;
      } else {
        return false;
      }
    }
  }

  // up-right
  for (auto r = king_rank - 1, f = king_file + 1; r >= 0 && f < 8; --r, ++f) {
    const auto upright = make_square(f, r);
    if (const auto s = is_safe(upright, dangerous_diagonal_piece); s != safe::unknown) {
      if (s == safe::yes) {
        break;
      } else {
        return false;
      }
    }
  }

  // down-left
  for (auto r = king_rank + 1, f = king_file - 1; r < 8 && f >= 0; ++r, --f) {
    const auto downleft = make_square(f, r);
    if (const auto s = is_safe(downleft, dangerous_diagonal_piece); s != safe::unknown) {
      if (s == safe::yes) {
        break;
      } else {
        return false;
      }
    }
  }

  // down-right
  for (auto r = king_rank + 1, f = king_file + 1; r < 8 && f < 8; ++r, ++f) {
    const auto downright = make_square(f, r);
    if (const auto s = is_safe(downright, dangerous_diagonal_piece); s != safe::unknown) {
      if (s == safe::yes) {
        break;
      } else {
        return false;
      }
    }
  }

  // surrounding knight squares
  for (const auto [r, f] : knight_offsets) {
    if (king_rank + r > 7 || king_rank + r < 0 || king_file + f > 7 || king_file + f < 0) {
      continue;
    }

    const auto knight_square = make_square(king_file + f, king_rank + r);
    const auto p = get_piece(b, knight_square);
    if (knight_square != to && p.type() == ptype::knight && p.colour() != b.active) {
      return false;
    }
  }

  return true;
}

// functor that will add a move to the move vector if it the active king is not
// under threat after such a move is made.
class move_generator {
public:
  explicit move_generator(const board &b) : b{b}, king{find_king(b)}, moves{} {}

  auto add_move(const square from, const square to) -> void {
    if (king_is_safe(b, king, from, to)) {
      moves.emplace_back(from, to);
    }
  }

  auto add_move(const square from, const square to, const ptype promotion) -> void {
    if (king_is_safe(b, king, from, to)) {
      moves.emplace_back(from, to, promotion);
    }
  }

  auto add_king_move(const square from, const square to) -> void {
    if (king_is_safe(b, to, square::_, square::_)) {
      moves.emplace_back(from, to);
    }
  }

  auto release() {
    return std::move(moves);
  }

private:
  const board &b;
  square king;
  std::vector<move> moves;
};

auto find_legal_pawn_moves(const square s,
                           const board &b,
                           move_generator &moves) -> void {
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

  const auto add_pawn_moves = [&](const square new_square) {
    const std::uint8_t top_rank = pawn.colour() == colour::white ? 7 : 0;

    if (rank(new_square) == top_rank) {
      moves.add_move(s, new_square, ptype::rook);
      moves.add_move(s, new_square, ptype::knight);
      moves.add_move(s, new_square, ptype::bishop);
      moves.add_move(s, new_square, ptype::queen);
    } else {
      moves.add_move(s, new_square);
    }
  };

  // move up one
  const auto up1 = move_by_rank(s, direction);
  if (get_piece(b, up1) == pieces::_) {
    add_pawn_moves(up1);

    // move up two
    if (rank(s) == second_rank) {
      const auto up2 = move_by_rank(up1, direction);
      if (get_piece(b, up2) == pieces::_) {
        moves.add_move(s, up2);
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
      add_pawn_moves(up1left1);
    }
  }

  // take a piece right
  if (file(up1) < 7) {
    const auto up1right1 = move_by_file(up1, 1);
    if (can_take(up1right1)) {
      add_pawn_moves(up1right1);
    }
  }
}

auto find_legal_rook_moves(const square s,
                           const board &b,
                           move_generator &moves) -> void {
  // legal rook moves:
  //   from where the piece is, walk up and down the file until you find another
  //   piece -- include that square if it is an enemy piece. ditto walking left
  //   and right on the rank.
  const auto rook_colour = get_piece(b, s).colour();
  const std::int8_t rook_rank = rank(s);
  const std::int8_t rook_file = file(s);

  // adds a rook move, returns false if a direction should not be continued
  const auto add_rook_move = [&](const square new_square) -> bool {
    const auto other_piece = get_piece(b, new_square);
    if (other_piece == pieces::_) {
      moves.add_move(s, new_square);
      return true;
    } else if (rook_colour != other_piece.colour()) {
      moves.add_move(s, new_square);
      return false;
    } else {
      assert(rook_colour == other_piece.colour());
      return false;
    }
  };

  // up
  for (auto r = rook_rank - 1; r >= 0; --r) {
    const auto up = make_square(rook_file, r);
    if (!add_rook_move(up)) {
      break;
    }
  }

  // down
  for (auto r = rook_rank + 1; r < 8; ++r) {
    const auto down = make_square(rook_file, r);
    if (!add_rook_move(down)) {
      break;
    }
  }

  // left
  for (auto f = rook_file - 1; f >= 0; --f) {
    const auto left = make_square(f, rook_rank);
    if (!add_rook_move(left)) {
      break;
    }
  }

  // right
  for (auto f = rook_file + 1; f < 8; ++f) {
    const auto right = make_square(f, rook_rank);
    if (!add_rook_move(right)) {
      break;
    }
  }
}

auto find_legal_knight_moves(const square s,
                             const board &b,
                             move_generator &moves) -> void {
  // legal knight moves:
  //   there is always 8 squares around the knight it can go as long as they
  //   are in the bounds of the board and not occupied by a friendly piece
  const auto knight_colour = get_piece(b, s).colour();

  const std::int8_t r = rank(s);
  const std::int8_t f = file(s);

  for (const auto [ro, fo] : knight_offsets) {
    if (r + ro > 7 || r + ro < 0 || f + fo > 7 || f + fo < 0) {
      continue;
    }

    const auto new_square = make_square(f + fo, r + ro);
    const auto other_piece = get_piece(b, new_square);
    if (other_piece == pieces::_ || other_piece.colour() != knight_colour) {
      moves.add_move(s, new_square);
    } else {
      assert(other_piece.colour() == knight_colour);
    }
  }
}

auto find_legal_bishop_moves(const square s,
                             const board &b,
                             move_generator &moves) -> void {
  // legal bishop moves:
  //   similar to the rook moves, except it walks diagonally in each axis.
  const auto bishop_colour = get_piece(b, s).colour();
  const std::int8_t bishop_rank = rank(s);
  const std::int8_t bishop_file = file(s);

  // adds a bishop move, returns false if a direction should not be continued
  const auto add_move = [&](const square new_square) -> bool {
    const auto other_piece = get_piece(b, new_square);
    if (other_piece == pieces::_) {
      moves.add_move(s, new_square);
      return true;
    } else if (bishop_colour != other_piece.colour()) {
      moves.add_move(s, new_square);
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

auto find_legal_king_moves(const square s,
                           const board &b,
                           move_generator &moves) -> void {
  // legal king moves:
  //   one in every direction
  //   TODO: castling
  const auto king_colour = get_piece(b, s).colour();

  std::array<P, 8> offsets{
    P(-1, 1), P(0, 1), P(1, 1), P(-1, 0), P(1, 0), P(-1, -1), P(0, -1), P(1, -1)
  };

  const std::int8_t r = rank(s);
  const std::int8_t f = file(s);

  for (const auto [ro, fo] : offsets) {
    if (r + ro > 7 || r + ro < 0 || f + fo > 7 || f + fo < 0) {
      continue;
    }

    const auto new_square = make_square(f + fo, r + ro);
    const auto other_piece = get_piece(b, new_square);
    if (other_piece == pieces::_ || other_piece.colour() != king_colour) {
      moves.add_king_move(s, new_square);
    } else {
      assert(other_piece.colour() == king_colour);
    }
  }
}

} // unnamed namespace

auto find_legal_moves(const board &b) -> std::vector<move> {
  move_generator moves{b};

  // walk through board, for each piece who's turn it is populate the moves
  // vector with their legal moves.
  for (unsigned i = 0; i < b.piece_board.size(); ++i) {
    const auto p = b.piece_board[i];
    const auto s = static_cast<square>(i);

    if (p.colour() == b.active) {
      switch(p.type()) {
        case ptype::pawn:
          find_legal_pawn_moves(s, b, moves);
          break;
        case ptype::rook:
          find_legal_rook_moves(s, b, moves);
          break;
        case ptype::knight:
          find_legal_knight_moves(s, b, moves);
          break;
        case ptype::bishop:
          find_legal_bishop_moves(s, b, moves);
          break;
        case ptype::queen:
          find_legal_rook_moves(s, b, moves);
          find_legal_bishop_moves(s, b, moves);
          break;
        case ptype::king:
          find_legal_king_moves(s, b, moves);
          break;
        case ptype::_:
          break;
      }
    }
  }

  return moves.release();
}

} // namespace pawntificate
