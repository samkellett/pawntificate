#ifndef PAWNTIFICATE_EVALUATE_HPP
#define PAWNTIFICATE_EVALUATE_HPP

#include <cstddef>

namespace pawntificate {

struct board;

class move;

constexpr std::size_t default_depth = 7ul;

// for a given board, return the strongest move in UCI format.
auto evaluate(const board &b, std::size_t depth = default_depth) -> move;

} // namespace pawntificate

#endif // PAWNTIFICATE_EVALUATE_HPP
