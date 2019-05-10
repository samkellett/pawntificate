#include "pawntificate/evaluate.hpp"

#include "pawntificate/board.hpp"

#include <random>
#include <sstream>

namespace pawntificate {

auto evaluate(const board &b) -> std::string {
  // just find all legal moves and pick a random one.
  const auto moves = find_legal_moves(b);

  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<> dis(0, moves.size() - 1);

  std::stringstream ss;
  ss << moves.at(dis(gen));
  return ss.str();
}

} // namespace pawntificate
