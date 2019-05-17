// UCI front-end for the pawntificate engine.
#include <filesystem>
#include <iostream>

#include <cxx/random.hpp>

#include <pawntificate/board.hpp>
#include <pawntificate/evaluate.hpp>
#include <pawntificate/uci_command.hpp>

int main() {
  static const auto rng_dump = std::filesystem::temp_directory_path() / "pawntificate.rng";
  auto rng = cxx::make_random_engine<std::mt19937>();

  // for debugging purposes we dump the rng state to disk before each evaluation
  std::cout << "info string rng state stored at '" << rng_dump << "' before each move" << std::endl;

  pawntificate::board board;

  // uci commands arrive from stdin.
  pawntificate::uci_command input;

  while (true) {
    // block until a command is received.
    input.read_line(std::cin);

    const auto cmd = input.next_token();
    if (cmd == "uci") {
      std::cout << "id name pawntificate\n"
                << "uciok\n";
    } else if (cmd == "isready") {
      std::cout << "readyok\n";
    } else if (cmd == "position") {
      const auto pos = input.next_token();
      if (pos != "startpos") {
        throw std::runtime_error("don't support custom position");
      }

      // after startpos there may be the token "moves" and a list of long
      // algebraic notation moves after that keyword if it exists. build up a
      // board representation from these moves.
      const auto move_list = [&]() -> std::string_view {
        if (input.next_token() == "moves") {
          return input.all_tokens();
        } else {
          return "";
        }
      }();
      board = pawntificate::board(move_list);
    } else if (cmd == "go") {
      cxx::serialise_random_engine(rng, rng_dump);

      // example format: go wtime 303000 btime 301750 winc 3000 binc 3000
      // all times are in milliseconds
      //const auto info = pawntificate::parse_move_info(input.all_tokens());

      // evaluate the last seen board position and return the best move.
      std::cout << "bestmove ";
      const auto bestmove = pawntificate::evaluate(board, rng);
      to_uci(std::cout, bestmove) << std::endl;
    } else if (cmd == "quit") {
      break;
    }
  }

  return 0;
}
