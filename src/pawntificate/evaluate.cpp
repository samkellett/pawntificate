#include "pawntificate/evaluate.hpp"

#include "pawntificate/board.hpp"

#include <random>

namespace pawntificate {

namespace {

using score = int;

// find legal moves and sort them so the best moves are first (probably). the order
// is derived from the killer bit being set or not.
auto find_and_sort_legal_moves(const board &b, std::mt19937 &gen) -> std::vector<move> {
  auto moves = find_legal_moves(b);

  auto begin = std::begin(moves);
  auto end = std::end(moves);

  // first put all killer moves to the start.
  auto killer_end = std::partition(begin, end, [](const move &m) {
    return m.killer();
  });

  // sort the killer moves by promotion.
  std::sort(begin, killer_end, [](const move &lhs, const move &rhs) {
    // strongest first.
    return !(lhs.promote_to() < rhs.promote_to());
  });

  // finally randomly shuffle the remaining moves.
  std::shuffle(killer_end, end, gen);

  return moves;
}

// for now we just do a basic count of the pieces using the normal weighting.
auto evaluate_position(const board &b) -> score {
  score s = 0;

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

function alphabeta(node, depth, α, β, maximizingPlayer) is
    if depth = 0 or node is a terminal node then
        return the heuristic value of node
    if maximizingPlayer then
        value := −∞
        for each child of node do
            value := max(value, alphabeta(child, depth − 1, α, β, FALSE))
            α := max(α, value)
            if α ≥ β then
                break (* β cut-off *)
        return value
    else
        value := +∞
        for each child of node do
            value := min(value, alphabeta(child, depth − 1, α, β, TRUE))
            β := min(β, value)
            if α ≥ β then
                break (* α cut-off *)
        return value

*/

struct variation {
  score score;
  move move;
};

// find the best variation by searching to a fixed depth.
auto alphabeta(const board &b,
               const move m,
               std::size_t depth,
               score alpha,
               score beta,
               const bool maximising,
               bool move_count_pruning,
               std::mt19937 &gen) -> variation {
  if (depth == 0) {
    const auto s = evaluate_position(b);
    return {maximising ? s : -s, m};
  }

  // late move reduction for non-killer moves below a certain depth.
  if (!move_count_pruning &&
      !m.killer() && m.promote_to() == ptype::_ &&
      depth >= 2 && depth <= 4) {
    depth -= 2;
    move_count_pruning = true;
  } else {
    --depth;
  }

  // find the legal moves. if there are none (ie. checkmate) this will naturally
  // terminate the search at this depth with a low score.
  // TODO: there is a bug here that makes stalemate and checkmate equivelent
  // which can cause the engine to throw away a winning position.
  const auto moves = find_and_sort_legal_moves(b, gen);

  if (maximising) {
    variation value{std::numeric_limits<score>::min(), m};
    for (const auto next_move : moves) {
      const auto next_value = alphabeta(board{b, next_move},
                                        next_move,
                                        depth,
                                        alpha,
                                        beta,
                                        false,
                                        move_count_pruning,
                                        gen);
      value.score = std::max(value.score, next_value.score);
      alpha = std::max(alpha, value.score);
      if (alpha >= beta) {
        break;
      }
    }

    return value;
  } else {
    variation value{std::numeric_limits<score>::max(), m};
    for (const auto next_move : moves) {
      const auto next_value = alphabeta(board{b, next_move},
                                        next_move,
                                        depth,
                                        alpha,
                                        beta,
                                        true,
                                        move_count_pruning,
                                        gen);
      value.score = std::min(value.score, next_value.score);
      beta = std::min(beta, value.score);
      if (alpha >= beta) {
        break;
      }
    }

    return value;
  }
}

// entry point: find all legal moves, find the best move for each one and return that.
auto alphabeta(const board &b, const std::size_t depth, std::mt19937 &gen) -> variation {
  const auto moves = find_and_sort_legal_moves(b, gen);
  assert(!moves.empty());

  const score alpha = std::numeric_limits<score>::min();
  const score beta = std::numeric_limits<score>::max();

  variation value = alphabeta(board{b, moves[0]},
                              moves[0],
                              depth - 1,
                              alpha,
                              beta,
                              false,
                              false,
                              gen);
  for (auto i = 1ul; i < moves.size(); ++i) {
    const auto m = moves[i];
    const auto v = alphabeta(board{b, m},
                             m,
                             depth - 1,
                             alpha,
                             beta,
                             false, 
                             false,
                             gen);

    value = std::max(value, v, [](const variation &lhs, const variation &rhs) {
      return lhs.score < rhs.score;
    });
  }

  return value;
}

} // unnamed namespace

auto evaluate(const board &b, const std::size_t depth) -> move {
  // TODO: eventually it would be nice to remove all randomness from the board
  // evaluation... it basically exists to hide any holes we have.
  std::mt19937 gen;

  const auto best_variation = alphabeta(b, depth, gen);
  return best_variation.move;
}

} // namespace pawntificate
