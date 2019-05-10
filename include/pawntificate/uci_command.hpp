#ifndef PAWNTIFICATE_UCI_COMMAND_HPP
#define PAWNTIFICATE_UCI_COMMAND_HPP

#include <iostream>
#include <string>
#include <string_view>

namespace pawntificate {

class uci_command {
public:
  uci_command() = default;

  // populate the internal buffer with stdin, blocks until something is received.
  auto read_line(std::istream &in) -> void {
    n = 0;
    cmd.clear();
    std::getline(in, cmd);
  }

  // returns the next token (whitespace deliminated), if there is one.
  auto next_token() -> std::string_view {
    if (n == std::string::npos) {
      return "";
    }

    // find the next whitespace character.
    const auto next = cmd.find(" ", n);
    if (next == std::string::npos) {
      const std::string_view token{cmd.data() + n};
      n = next;
      return token;
    }

    // return a view of the current token.
    const auto size = next - n;
    std::string_view token{cmd.data() + n, size};
    n = next + 1;
    return token;
  }

  // consume all of the remaining tokens in a single view.
  auto all_tokens() -> std::string_view {
    if (n == std::string::npos) {
      return {};
    } else {
      const std::string_view tokens{cmd.data() + n};
      n = std::string::npos;
      return tokens;
    }
  }

private:
  std::size_t n = std::string::npos;
  std::string cmd;
};

} // namespace pawntificate

#endif // PAWNTIFICATE_UCI_COMMAND_HPP
