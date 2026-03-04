#include <iostream>
#include "parser.hpp"

std::vector<Token> lex(const std::string& input) {
  std::vector<Token> tokens;
  size_t i = 0;
  while (i < input.size()) {
    char c = input.at(i);
    if (c == ' ') { i++; continue; }
    if (std::isdigit(c)) {
      std::string n;
      while (i < input.size() && std::isdigit(input.at(i))) {
        n.push_back(input.at(i));
        i++;
      };
      tokens.push_back(Token { TokenType::Integer, std::move(n) });
      continue;
    };
    if (input.substr(i, 2) == "if") {
      tokens.push_back(Token { TokenType::If, "if" });
      i = i + 2;
      continue;
    }
    if (input.substr(i, 4) == "then") {
      tokens.push_back(Token { TokenType::Then, "Then" });
      i = i + 4;
      continue;
    }
    if (input.substr(i, 4) == "else") {
      tokens.push_back(Token { TokenType::Else, "else" });
      i = i + 4;
      continue;
    }
    if (input.substr(i, 5) == "while") {
      tokens.push_back(Token { TokenType::While, "while" });
      i = i + 5;
      continue;
    }
    if (input.substr(i, 2) == "do") {
      tokens.push_back(Token { TokenType::Do, "do" });
      i = i + 2;
      continue;
    }
    if (input.substr(i, 4) == "true") {
      tokens.push_back(Token { TokenType::True, "true" });
      i = i + 4;
      continue;
    }
    if (input.substr(i, 5) == "false") {
      tokens.push_back(Token { TokenType::False, "false" });
      i = i + 5;
      continue;
    }
    if (std::isalpha(c)) {
      std::string s;
      while (i < input.size() && std::isalpha(input.at(i))) {
        s.push_back(input.at(i));
        i++;
      };
      tokens.push_back(Token { TokenType::Identifier, std::move(s) });
      continue;
    }
    switch (c) {
      case '#': tokens.push_back(Token { TokenType::Skip, "#" }); break;
      case '+': tokens.push_back(Token { TokenType::Plus, "+" }); break;
      case '-': tokens.push_back(Token { TokenType::Minus, "-" }); break;
      case '=': tokens.push_back(Token { TokenType::Equal, "=" }); break;
      case '>': tokens.push_back(Token { TokenType::Greater_than, ">" }); break;
      case ';': tokens.push_back(Token { TokenType::Semi_colon, ";" }); break;
      default:  throw std::runtime_error("Unexpected character"); break;
    }
    i++;
  }
  tokens.push_back( Token {TokenType::End_of_file, ""});
  return tokens;
}
