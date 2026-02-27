enum class TokenType {
  Integer,
  Plus,
  Minus,
  End,
};

struct Token {
  TokenType type;
  std::string text;
};

std::vector<Token> lex(const std::string& input);
