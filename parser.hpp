#pragma once
#include <vector>

enum class TokenType {
  Integer,
  Plus, Minus,
  Identifier,
  True, False,
  Greater_than, Equal,
  If, Then, Else,
  While, Do,
  Semi_colon,
  Skip,
  End_of_file,
};

struct Token {
  TokenType type;
  std::string text;
};

std::vector<Token> lex(const std::string& input);
