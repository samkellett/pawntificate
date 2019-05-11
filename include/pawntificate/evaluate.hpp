#ifndef PAWNTIFICATE_EVALUATE_HPP
#define PAWNTIFICATE_EVALUATE_HPP

namespace pawntificate {

struct board;

class move;

// for a given board, return the strongest move in UCI format.
auto evaluate(const board &b) -> move;

} // namespace pawntificate

#endif // PAWNTIFICATE_EVALUATE_HPP
