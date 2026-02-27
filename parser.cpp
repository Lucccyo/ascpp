#include <iostream>
#include "parser.hpp"

std::vector<Token> lex(const std::string& input) {
  std::vector<Token> tokens;
  size_t i = 0;
  while (i < input.size()) {
    char c = input.at(i);
    if (c == ' ') {
      i++;
      continue;
    };

    if (std::isdigit(c)) {
      std::string n;
      while (i < input.size() && std::isdigit(input.at(i))) {
        n.push_back(input.at(i));
        i++;
      };
      std::cout << "new token int : " << n << '\n';
      tokens.push_back(Token {
        TokenType::Integer, std::move(n)
      });
      continue;
    };

    if (c == '+') {
      std::cout << "new token plus : + " << '\n';
      tokens.push_back(Token {
        TokenType::Plus, "+"
      });
    } else
    if (c == '-') {
      std::cout << "new token minus : - " << '\n';
      tokens.push_back(Token {
        TokenType::Minus, "-"
      });
    } else {
      throw std::runtime_error("Unexpected character");
    };
    i++;
  };
  tokens.push_back( Token {TokenType::End, ""});
  return tokens;
}
