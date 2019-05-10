#ifndef PAWNTIFICATE_EVALUATE_HPP
#define PAWNTIFICATE_EVALUATE_HPP

#include <string>

namespace pawntificate {

struct board;

// for a given board, return the strongest move in UCI format.
auto evaluate(const board &b) -> std::string;

} // namespace pawntificate

#endif // PAWNTIFICATE_EVALUATE_HPP
