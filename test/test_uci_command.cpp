#include <gtest/gtest.h>

#include <pawntificate/uci_command.hpp>

#include <sstream>

TEST(UciCommand, SingleToken) {
  pawntificate::uci_command command;

  const std::string input = "uci";
  std::istringstream in(input);

  ASSERT_EQ(command.next_token(), "");
  command.read_line(in);

  ASSERT_EQ(command.next_token(), "uci");
  ASSERT_EQ(command.next_token(), "");
  ASSERT_EQ(command.next_token(), "");

  ASSERT_EQ(command.all_tokens(), "");
}

TEST(UciCommand, TwoTokens) {
  pawntificate::uci_command command;

  const std::string input = "position startpos";
  std::istringstream in(input);

  ASSERT_EQ(command.all_tokens(), "");
  command.read_line(in);

  ASSERT_EQ(command.next_token(), "position");
  ASSERT_EQ(command.next_token(), "startpos");
  ASSERT_EQ(command.all_tokens(), "");
}

TEST(UciCommand, MultipleTokens) {
  pawntificate::uci_command command;

  const std::string input = "position startpos moves e2e4";
  std::istringstream in(input);

  command.read_line(in);
  ASSERT_EQ(command.next_token(), "position");
  ASSERT_EQ(command.next_token(), "startpos");
  ASSERT_EQ(command.next_token(), "moves");
  ASSERT_EQ(command.next_token(), "e2e4");
  ASSERT_EQ(command.next_token(), "");
}

TEST(UciCommand, MultipleTokensAllTokens) {
  pawntificate::uci_command command;

  const std::string input = "position startpos moves e2e4 c7c5";
  std::istringstream in(input);

  command.read_line(in);
  ASSERT_EQ(command.next_token(), "position");
  ASSERT_EQ(command.next_token(), "startpos");
  ASSERT_EQ(command.next_token(), "moves");
  ASSERT_EQ(command.all_tokens(), "e2e4 c7c5");
  ASSERT_EQ(command.next_token(), "");
}
