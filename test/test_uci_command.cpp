#include <gtest/gtest.h>

#include <pawntificate/uci_command.hpp>

#include <sstream>

TEST(UciCommand, SingleToken) {
  pawntificate::uci_command command;

  const std::string input = "uci";
  std::istringstream in(input);

  const auto cmd = command.read_line(in);
  ASSERT_EQ(cmd, "uci");
  // ASSERT_EQ(command.next_token(), "");
}

TEST(UciCommand, TwoTokens) {
  pawntificate::uci_command command;

  const std::string input = "position startpos";
  std::istringstream in(input);

  const auto cmd = command.read_line(in);
  ASSERT_EQ(cmd, "position");
  ASSERT_EQ(command.next_token(), "startpos");
  // ASSERT_EQ(std::string(command.next_token()), "");
}

TEST(UciCommand, MultipleTokens) {
  pawntificate::uci_command command;

  const std::string input = "position startpos moves e2e4";
  std::istringstream in(input);

  const auto cmd = command.read_line(in);
  ASSERT_EQ(cmd, "position");
  ASSERT_EQ(command.next_token(), "startpos");
  ASSERT_EQ(command.next_token(), "moves");
  ASSERT_EQ(command.next_token(), "e2e4");
  // ASSERT_EQ(command.next_token(), "");
}
