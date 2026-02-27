#include <iostream>
#include <variant>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "expr.hpp"

std::unordered_map<std::string, Sign> sign_table;

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
