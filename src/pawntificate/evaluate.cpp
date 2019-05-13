#include "pawntificate/evaluate.hpp"

#include "pawntificate/board.hpp"

#include <random>

namespace pawntificate {

namespace {

using score = int;

// for now we just do a basic count of the pieces using the normal weighting.
auto evaluate_position(const board &b) -> score {
  score s = 0;

  // maximise the not active colour as the board has moved on already.
  const auto multiplier = [&](const colour c) -> score {
    return c == b.active ? 1 : -1;
  };

  for (const auto p : b.piece_board) {
    const auto m = multiplier(p.colour());
    switch(p.type()) {
      case ptype::pawn:
        s += 1 * m;
        break;
      case ptype::rook:
        s += 5 * m;
        break;
      case ptype::knight:
      case ptype::bishop:
        s += 3 * m;
        break;
      case ptype::queen:
        s += 8 * m;
        break;
      case ptype::king:
        // king has no score as he can never be removed.
      case ptype::_:
        break;
    }
  }

  return s;
}

/*

function minimax(node, depth, maximizingPlayer) is
    if depth = 0 or node is a terminal node then
        return the heuristic value of node
    if maximizingPlayer then
        value := −∞
        for each child of node do
            value := max(value, minimax(child, depth − 1, FALSE))
        return value
    else (* minimizing player *)
        value := +∞
        for each child of node do
            value := min(value, minimax(child, depth − 1, TRUE))
        return value

*/

struct variation {
  score score;
  move move;
};

// find the best variation by searching to a fixed depth.
auto minimax(const board &b,
             const move m,
             const std::size_t depth,
             const bool maximising) -> variation {
  if (depth == 0) {
    return {evaluate_position(b), m};
  }

  // find the legal moves. if there are none (ie. checkmate) terminate the
  // search at this depth with a low score.
  const auto moves = find_legal_moves(b);
  if (moves.empty()) {
    return {std::numeric_limits<score>::min(), m};
  }

  if (maximising) {
    variation value{std::numeric_limits<score>::min(), m};
    for (const auto next_move : moves) {
      const auto next_value = minimax(board{b, next_move}, next_move, depth - 1, false);
      value.score = std::max(value.score, next_value.score);
    }

    return value;
  } else {
    variation value{std::numeric_limits<score>::max(), m};
    for (const auto next_move : moves) {
      const auto next_value = minimax(board{b, next_move}, next_move, depth - 1, true);
      value.score = std::min(value.score, next_value.score);
    }

    return value;
  }
}

// entry point: find all legal moves, find the best move for each one and return that.
auto minimax(const board &b, const std::size_t depth) -> variation {
  const auto moves = find_legal_moves(b);
  assert(!moves.empty());

  variation value{std::numeric_limits<score>::min(), {}};
  for (const auto m : moves) {
    const auto v = minimax(board{b, m}, m, depth - 1, false);
    value = std::max(value, v, [](const variation &lhs, const variation &rhs) {
      return lhs.score < rhs.score;
    });
  }

  return value;
}

} // unnamed namespace

auto evaluate(const board &b) -> move {
  constexpr std::size_t depth = 3;

  const auto best_variation = minimax(b, depth);
  return best_variation.move;
}

} // namespace pawntificate
