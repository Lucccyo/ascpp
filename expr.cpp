#include <iostream>
#include <variant>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "expr.hpp"

std::unordered_map<std::string, Sign> sign_table;

Expr parse_factor(const std::vector<Token>& tokens, size_t& i) {
  if (tokens[i].type == TokenType::Minus) {
    i++;
    Expr inner = parse_factor(tokens, i);
    return Expr { std::make_unique<Neg>(Neg{std::move(inner)})};
  }
  if (tokens[i].type == TokenType::Integer) {
    int value = std::stoi(tokens[i].text);
    i++;
    return Expr {Int {value}};
  }
  throw std::runtime_error("Unexpected token in factor");
}

Expr parse_expr(const std::vector<Token>& tokens) {
  size_t i = 0;
  Expr left = parse_factor(tokens, i);
  while (i < tokens.size() && (tokens[i].type == TokenType::Plus || tokens[i].type == TokenType::Minus)) {
    TokenType op = tokens[i].type;
    i++;
    Expr right = parse_factor(tokens, i);
    if (op == TokenType::Plus) {
      left = Expr { std::make_unique<Add>(Add{std::move(left), std::move(right)})};
    }
    if (op == TokenType::Minus) {
      left = Expr{ std::make_unique<Add>(Add{ std::move(left), std::make_unique<Neg>(Neg{std::move(right)}) }) };
    }
  }
  return left;
}

Sign sign_of_expr(const Expr& e) {
  if (std::holds_alternative<Int>(e)) {
    const Int& i = std::get<Int>(e);
    if (i.value < 0) return Sign::NEG;
    if (i.value > 0) return Sign::POS;
    return Sign::ZER;
  };
  if (std::holds_alternative<Val>(e)) {
    const Val& v = std::get<Val>(e);
    return sign_table[v.name];
  };
  if (std::holds_alternative<std::unique_ptr<Add>>(e)) {
    const Add& a = *std::get<std::unique_ptr<Add>>(e);
    return add(sign_of_expr(a.left), sign_of_expr(a.right));
  };
  if (std::holds_alternative<std::unique_ptr<Neg>>(e)) {
    const Neg& n = *std::get<std::unique_ptr<Neg>>(e);
    return negate(sign_of_expr(n.expr));
  };
  return Sign::BOT;
}
