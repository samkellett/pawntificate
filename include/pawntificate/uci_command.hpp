#ifndef PAWNTIFICATE_UCI_COMMAND_HPP
#define PAWNTIFICATE_UCI_COMMAND_HPP

#include <iostream>
#include <string>
#include <string_view>

namespace pawntificate {

class uci_command {
public:
  uci_command() = default;

  // populate the internal buffer with stdin, blocks until something is
  // received. returns the first token inside the input.
  std::string_view read_line(std::istream &in) {
    cmd.clear();
    std::getline(in, cmd);

    n = cmd.find(" ", 0);
    if (n == std::string::npos) {
      return cmd;
    } else {
      return std::string_view{cmd.data(), n++};
    }
  }

  // returns the next token (whitespace deliminated), if there is one.
  std::string_view next_token() {
    // if (n == std::string::npos) {
    //   return std::string_view{""};
    // }

    // find the next whitespace character.
    const auto next = cmd.find(" ", n);
    if (next == std::string::npos) {
      // n = next;
      return std::string_view{cmd.data() + n};
    }

    const auto size = next - n;

    // return a view of the current token.
    std::string_view token{cmd.data() + n, size};
    n = next + 1;
    return token;
  }

  // consume all of the remaining tokens in a single view.
  std::string_view all_tokens() {
    return {};
  }

private:
  std::size_t n = 0;
  std::string cmd;
};

} // namespace pawntificate

#endif // PAWNTIFICATE_UCI_COMMAND_HPP
